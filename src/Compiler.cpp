#include "Compiler.h"
#include <stdexcept>
#include <cstring>

Compiler::Compiler() {}

std::vector<uint8_t> Compiler::compile(const std::vector<std::unique_ptr<Stmt>>& stmts) {
    for (const auto& stmt : stmts) {
        compileStmt(stmt.get());
    }
    emitByte(static_cast<uint8_t>(Opcode::HALT));
    return bytecode;
}

void Compiler::emitByte(uint8_t byte) {
    bytecode.push_back(byte);
}

void Compiler::emitInt64(int64_t value) {
    for (int i = 0; i < 8; ++i) {
        bytecode.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
    }
}

void Compiler::emitInt32(int32_t value) {
    for (int i = 0; i < 4; ++i) {
        bytecode.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
    }
}

size_t Compiler::emitJump(Opcode instruction) {
    emitByte(static_cast<uint8_t>(instruction));
    emitInt32(0); // Placeholder
    return bytecode.size() - 4;
}

void Compiler::patchJump(size_t offset) {
    int32_t jump = static_cast<int32_t>(bytecode.size() - offset - 4);
    for (int i = 0; i < 4; ++i) {
        bytecode[offset + i] = static_cast<uint8_t>((jump >> (i * 8)) & 0xFF);
    }
}

uint8_t Compiler::resolveVariable(const std::string& name) {
    if (globals.find(name) == globals.end()) {
        throw std::runtime_error("Undefined variable: " + name);
    }
    return globals[name];
}

uint8_t Compiler::declareVariable(const std::string& name) {
    if (globals.find(name) != globals.end()) {
        throw std::runtime_error("Variable already declared: " + name);
    }
    if (nextVarIdx >= 255) {
        throw std::runtime_error("Too many variables");
    }
    globals[name] = nextVarIdx;
    return nextVarIdx++;
}

void Compiler::compileStmt(Stmt* stmt) {
    if (auto* s = dynamic_cast<LetStmt*>(stmt)) {
        compileExpr(s->initializer.get());
        uint8_t idx = declareVariable(s->name.lexeme);
        emitByte(static_cast<uint8_t>(Opcode::STORE_VAR));
        emitByte(idx);
        emitByte(static_cast<uint8_t>(Opcode::POP)); // Assignment result not needed
    } else if (auto* s = dynamic_cast<AssignStmt*>(stmt)) {
        compileExpr(s->value.get());
        uint8_t idx = resolveVariable(s->name.lexeme);
        emitByte(static_cast<uint8_t>(Opcode::STORE_VAR));
        emitByte(idx);
        emitByte(static_cast<uint8_t>(Opcode::POP));
    } else if (auto* s = dynamic_cast<PrintStmt*>(stmt)) {
        compileExpr(s->expression.get());
        emitByte(static_cast<uint8_t>(Opcode::PRINT));
    } else if (auto* s = dynamic_cast<ExprStmt*>(stmt)) {
        compileExpr(s->expression.get());
        emitByte(static_cast<uint8_t>(Opcode::POP));
    } else if (auto* s = dynamic_cast<BlockStmt*>(stmt)) {
        for (const auto& inner : s->statements) {
            compileStmt(inner.get());
        }
    } else if (auto* s = dynamic_cast<IfStmt*>(stmt)) {
        compileExpr(s->condition.get());
        size_t thenJump = emitJump(Opcode::JUMP_IF_FALSE);
        emitByte(static_cast<uint8_t>(Opcode::POP)); // pop condition
        compileStmt(s->thenBranch.get());
        
        size_t elseJump = emitJump(Opcode::JUMP);
        patchJump(thenJump);
        emitByte(static_cast<uint8_t>(Opcode::POP)); // pop condition for false branch
        
        if (s->elseBranch) {
            compileStmt(s->elseBranch.get());
        }
        patchJump(elseJump);
    } else if (auto* s = dynamic_cast<WhileStmt*>(stmt)) {
        size_t loopStart = bytecode.size();
        compileExpr(s->condition.get());
        size_t exitJump = emitJump(Opcode::JUMP_IF_FALSE);
        emitByte(static_cast<uint8_t>(Opcode::POP)); // pop condition
        
        compileStmt(s->body.get());
        
        // Emit jump backward
        emitByte(static_cast<uint8_t>(Opcode::JUMP));
        int32_t jumpDist = static_cast<int32_t>(bytecode.size() + 4 - loopStart);
        emitInt32(-jumpDist);
        
        patchJump(exitJump);
        emitByte(static_cast<uint8_t>(Opcode::POP)); // pop condition on exit
    }
}

void Compiler::compileExpr(Expr* expr) {
    if (auto* e = dynamic_cast<LiteralExpr*>(expr)) {
        if (e->value.type == TokenType::INT_LITERAL) {
            emitByte(static_cast<uint8_t>(Opcode::PUSH_INT));
            emitInt64(std::stoll(e->value.lexeme));
        } else if (e->value.type == TokenType::BOOL_LITERAL) {
            emitByte(static_cast<uint8_t>(Opcode::PUSH_BOOL));
            emitByte(e->value.lexeme == "true" ? 1 : 0);
        }
    } else if (auto* e = dynamic_cast<VariableExpr*>(expr)) {
        uint8_t idx = resolveVariable(e->name.lexeme);
        emitByte(static_cast<uint8_t>(Opcode::LOAD_VAR));
        emitByte(idx);
    } else if (auto* e = dynamic_cast<InputExpr*>(expr)) {
        emitByte(static_cast<uint8_t>(Opcode::INPUT));
    } else if (auto* e = dynamic_cast<BinaryExpr*>(expr)) {
        compileExpr(e->left.get());
        compileExpr(e->right.get());
        switch (e->op.type) {
            case TokenType::PLUS: emitByte(static_cast<uint8_t>(Opcode::ADD)); break;
            case TokenType::MINUS: emitByte(static_cast<uint8_t>(Opcode::SUB)); break;
            case TokenType::STAR: emitByte(static_cast<uint8_t>(Opcode::MUL)); break;
            case TokenType::SLASH: emitByte(static_cast<uint8_t>(Opcode::DIV)); break;
            case TokenType::EQ_EQ: emitByte(static_cast<uint8_t>(Opcode::EQ)); break;
            case TokenType::LESS: emitByte(static_cast<uint8_t>(Opcode::LT)); break;
            default: break;
        }
    }
}
