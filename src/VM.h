#pragma once
#include "Opcode.h"
#include <vector>
#include <cstdint>
#include <iostream>

struct Value {

    enum class Type {
        INT,
        BOOL
    } type;

    union{
        int64_t i;
        bool b;
    } as;

    void print() const{

        if(type==Type::INT)
            std::cout<<as.i;

        else
            std::cout
            <<(as.b?"true":"false");
    }
};

class VM {

public:

    VM();

    void execute(
    const std::vector<uint8_t>& bytecode
    );

    void setDebug(
    bool enabled
    );

private:

    std::vector<Value> stack;

    std::vector<Value> variables;

    bool debugMode=false;

    void push(
    Value value
    );

    Value pop();

    void printStack();

    void debugInstruction(
    const std::string& name
    );

    int64_t readInt64(
    const std::vector<uint8_t>& bytecode,
    size_t& ip
    );

    int32_t readInt32(
    const std::vector<uint8_t>& bytecode,
    size_t& ip
    );
};