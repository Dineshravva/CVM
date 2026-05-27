#include "VM.h"
#include <stdexcept>
#include <iostream>

VM::VM() : variables(256) {}

void VM::setDebug(bool enabled){
    debugMode=enabled;
}

void VM::debugInstruction(
const std::string& name
){
    if(!debugMode)
        return;

    std::cout
    <<"Executing: "
    <<name
    <<"\n";
}

void VM::printStack(){

    if(!debugMode)
        return;

    std::cout
    <<"Stack: [";

    for(size_t i=0;i<stack.size();++i){

        stack[i].print();

        if(i+1<stack.size())
            std::cout<<", ";
    }

    std::cout
    <<"]\n\n";
}

void VM::push(Value value){

    stack.push_back(value);
}

Value VM::pop(){

    if(stack.empty())
        throw std::runtime_error(
        "Stack underflow"
    );

    Value val=
    stack.back();

    stack.pop_back();

    return val;
}

int64_t VM::readInt64(
const std::vector<uint8_t>& bytecode,
size_t& ip
){
    int64_t val=0;

    for(int i=0;i<8;++i){

        val|=

        static_cast<int64_t>(
        bytecode[ip++]
        )
        <<(i*8);
    }

    return val;
}

int32_t VM::readInt32(
const std::vector<uint8_t>& bytecode,
size_t& ip
){
    int32_t val=0;

    for(int i=0;i<4;++i){

        val|=

        static_cast<int32_t>(
        bytecode[ip++]
        )
        <<(i*8);
    }

    return val;
}

void VM::execute(
const std::vector<uint8_t>& bytecode
){

    size_t ip=0;

    while(ip<bytecode.size()){

        Opcode op=
        static_cast<Opcode>(
        bytecode[ip++]
        );

        switch(op){

        case Opcode::PUSH_INT:{

            debugInstruction(
            "PUSH_INT"
            );

            int64_t val=
            readInt64(
            bytecode,
            ip
            );

            Value temp;
            temp.type=
            Value::Type::INT;
            temp.as.i=val;

            push(temp);

            printStack();

            break;
        }

        case Opcode::PUSH_BOOL:{

            debugInstruction(
            "PUSH_BOOL"
            );

            bool val=
            bytecode[ip++]!=0;

            Value temp;
            temp.type=
            Value::Type::BOOL;
            temp.as.b=val;

            push(temp);

            printStack();

            break;
        }

        case Opcode::ADD:{

            debugInstruction(
            "ADD"
            );

            Value b=pop();
            Value a=pop();

            Value temp;
            temp.type=
            Value::Type::INT;
            temp.as.i=
            a.as.i+b.as.i;

            push(temp);

            printStack();

            break;
        }

        case Opcode::SUB:{

            Value b=pop();
            Value a=pop();

            Value temp;
            temp.type=Value::Type::INT;
            temp.as.i=
            a.as.i-b.as.i;

            push(temp);

            printStack();

            break;
        }

        case Opcode::MUL:{

            Value b=pop();
            Value a=pop();

            Value temp;
            temp.type=Value::Type::INT;
            temp.as.i=
            a.as.i*b.as.i;

            push(temp);

            printStack();

            break;
        }

        case Opcode::DIV:{

            Value b=pop();
            Value a=pop();

            Value temp;
            temp.type=Value::Type::INT;
            temp.as.i=
            a.as.i/b.as.i;

            push(temp);

            printStack();

            break;
        }

        case Opcode::EQ:{

            Value b=pop();
            Value a=pop();

            Value temp;
            temp.type=
            Value::Type::BOOL;

            temp.as.b=
            (a.as.i==b.as.i);

            push(temp);

            printStack();

            break;
        }

        case Opcode::LT:{

            Value b=pop();
            Value a=pop();

            Value temp;
            temp.type=
            Value::Type::BOOL;

            temp.as.b=
            (a.as.i<b.as.i);

            push(temp);

            printStack();

            break;
        }

        case Opcode::STORE_VAR:{

            uint8_t idx=
            bytecode[ip++];

            variables[idx]=
            stack.back();

            break;
        }

        case Opcode::LOAD_VAR:{

            debugInstruction(
            "LOAD_VAR"
            );

            uint8_t idx=
            bytecode[ip++];

            push(
            variables[idx]
            );

            printStack();

            break;
        }

        case Opcode::JUMP_IF_FALSE:{

            int32_t offset=
            readInt32(
            bytecode,
            ip
            );

            Value cond=
            stack.back();

            if(!cond.as.b){

                ip+=offset;
            }

            break;
        }

        case Opcode::JUMP:{

            int32_t offset=
            readInt32(
            bytecode,
            ip
            );

            ip+=offset;

            break;
        }

        case Opcode::PRINT:{

            Value val=
            pop();

            val.print();

            std::cout
            <<std::endl;

            break;
        }

        case Opcode::INPUT:{

            int64_t val;

            std::cin>>val;

            Value temp;
            temp.type=
            Value::Type::INT;
            temp.as.i=val;

            push(temp);

            break;
        }

        case Opcode::POP:{

            pop();
            break;
        }

        case Opcode::HALT:
            return;
        }
    }
}