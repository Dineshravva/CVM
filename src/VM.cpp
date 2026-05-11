#include "VM.h"
#include <stdexcept>
#include <iostream>

VM::VM() : variables(256) {}

void VM::push(Value value) {
    stack.push_back(value);
}

Value VM::pop() {
    if (stack.empty()) throw std::runtime_error("Stack underflow");
    Value val = stack.back();
    stack.pop_back();
    return val;
}

int64_t VM::readInt64(const std::vector<uint8_t>& bytecode, size_t& ip) {
    int64_t val = 0;
    for (int i = 0; i < 8; ++i) {
        val |= static_cast<int64_t>(bytecode[ip++]) << (i * 8);
    }
    return val;
}

int32_t VM::readInt32(const std::vector<uint8_t>& bytecode, size_t& ip) {
    int32_t val = 0;
    for (int i = 0; i < 4; ++i) {
        val |= static_cast<int32_t>(bytecode[ip++]) << (i * 8);
    }
    return val;
}

void VM::execute(const std::vector<uint8_t>& bytecode) {
    size_t ip = 0;
    while (ip < bytecode.size()) {
        Opcode op = static_cast<Opcode>(bytecode[ip++]);
        
        switch (op) {
            case Opcode::PUSH_INT: {
                int64_t val = readInt64(bytecode, ip);
                push(Value{Value::Type::INT, {.i = val}});
                break;
            }
            case Opcode::PUSH_BOOL: {
                bool val = bytecode[ip++] != 0;
                push(Value{Value::Type::BOOL, {.b = val}});
                break;
            }
            case Opcode::ADD: {
                Value b = pop(); Value a = pop();
                push(Value{Value::Type::INT, {.i = a.as.i + b.as.i}});
                break;
            }
            case Opcode::SUB: {
                Value b = pop(); Value a = pop();
                push(Value{Value::Type::INT, {.i = a.as.i - b.as.i}});
                break;
            }
            case Opcode::MUL: {
                Value b = pop(); Value a = pop();
                push(Value{Value::Type::INT, {.i = a.as.i * b.as.i}});
                break;
            }
            case Opcode::DIV: {
                Value b = pop(); Value a = pop();
                push(Value{Value::Type::INT, {.i = a.as.i / b.as.i}});
                break;
            }
            case Opcode::EQ: {
                Value b = pop(); Value a = pop();
                bool res = false;
                if (a.type == Value::Type::INT && b.type == Value::Type::INT) res = (a.as.i == b.as.i);
                else if (a.type == Value::Type::BOOL && b.type == Value::Type::BOOL) res = (a.as.b == b.as.b);
                push(Value{Value::Type::BOOL, {.b = res}});
                break;
            }
            case Opcode::LT: {
                Value b = pop(); Value a = pop();
                push(Value{Value::Type::BOOL, {.b = a.as.i < b.as.i}});
                break;
            }
            case Opcode::STORE_VAR: {
                uint8_t idx = bytecode[ip++];
                variables[idx] = stack.back(); // keep on stack, caller will POP
                break;
            }
            case Opcode::LOAD_VAR: {
                uint8_t idx = bytecode[ip++];
                push(variables[idx]);
                break;
            }
            case Opcode::JUMP_IF_FALSE: {
                int32_t offset = readInt32(bytecode, ip);
                Value cond = stack.back(); // peek, don't pop, caller pops
                if (cond.type == Value::Type::BOOL && !cond.as.b) {
                    ip += offset;
                }
                break;
            }
            case Opcode::JUMP: {
                int32_t offset = readInt32(bytecode, ip);
                ip += offset;
                break;
            }
            case Opcode::PRINT: {
                Value val = pop();
                val.print();
                std::cout << std::endl;
                break;
            }
            case Opcode::INPUT: {
                int64_t val;
                std::cin >> val;
                push(Value{Value::Type::INT, {.i = val}});
                break;
            }
            case Opcode::POP: {
                pop();
                break;
            }
            case Opcode::HALT: {
                return;
            }
        }
    }
}
