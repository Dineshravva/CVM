#pragma once
#include "AST.h"
#include "Opcode.h"
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <string>

class Compiler {
public:
    Compiler();
    std::vector<uint8_t> compile(const std::vector<std::unique_ptr<Stmt>>& stmts);

private:
    std::vector<uint8_t> bytecode;
    std::unordered_map<std::string, uint8_t> globals;
    uint8_t nextVarIdx = 0;

    void compileStmt(Stmt* stmt);
    void compileExpr(Expr* expr);
    
    void emitByte(uint8_t byte);
    void emitInt64(int64_t value);
    void emitInt32(int32_t value);
    size_t emitJump(Opcode instruction);
    void patchJump(size_t offset);
    
    uint8_t resolveVariable(const std::string& name);
    uint8_t declareVariable(const std::string& name);
};
