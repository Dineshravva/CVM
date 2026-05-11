#pragma once
#include <cstdint>

enum class Opcode : uint8_t {
    PUSH_INT,       // followed by 8-byte int64_t
    PUSH_BOOL,      // followed by 1-byte 0 or 1
    ADD,
    SUB,
    MUL,
    DIV,
    EQ,
    LT,
    STORE_VAR,      // followed by 1-byte local index
    LOAD_VAR,       // followed by 1-byte local index
    JUMP_IF_FALSE,  // followed by 4-byte offset
    JUMP,           // followed by 4-byte offset
    PRINT,
    INPUT,
    POP,            // Pop stack top
    HALT
};
