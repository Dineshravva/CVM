#include "Disassembler.h"

#include <iostream>
#include <iomanip>

int64_t Disassembler::readInt64(
    const std::vector<uint8_t>& bytecode,
    size_t& ip
){
    int64_t val=0;

    for(int i=0;i<8;i++){

        val |=
        static_cast<int64_t>(bytecode[ip++])
        <<(i*8);
    }

    return val;
}

int32_t Disassembler::readInt32(
    const std::vector<uint8_t>& bytecode,
    size_t& ip
){
    int32_t val=0;

    for(int i=0;i<4;i++){

        val |=
        static_cast<int32_t>(bytecode[ip++])
        <<(i*8);
    }

    return val;
}

void Disassembler::print(
    const std::vector<uint8_t>& bytecode
){

    size_t ip=0;

    std::cout<<"\n----- DISASSEMBLY -----\n";

    while(ip<bytecode.size()){

        size_t current=ip;

        Opcode op=
        static_cast<Opcode>(bytecode[ip++]);

        std::cout
        <<std::setw(4)
        <<std::setfill('0')
        <<current
        <<" ";

        switch(op){

        case Opcode::PUSH_INT:{
            int64_t val=
            readInt64(bytecode,ip);

            std::cout
            <<"PUSH_INT     "
            <<val;
            break;
        }

        case Opcode::PUSH_BOOL:{
            bool val=
            bytecode[ip++];

            std::cout
            <<"PUSH_BOOL    "
            <<(val?"true":"false");

            break;
        }

        case Opcode::ADD:
            std::cout<<"ADD";
            break;

        case Opcode::SUB:
            std::cout<<"SUB";
            break;

        case Opcode::MUL:
            std::cout<<"MUL";
            break;

        case Opcode::DIV:
            std::cout<<"DIV";
            break;

        case Opcode::EQ:
            std::cout<<"EQ";
            break;

        case Opcode::LT:
            std::cout<<"LT";
            break;

        case Opcode::STORE_VAR:{

            int idx=
            bytecode[ip++];

            std::cout
            <<"STORE_VAR   "
            <<idx;

            break;
        }

        case Opcode::LOAD_VAR:{

            int idx=
            bytecode[ip++];

            std::cout
            <<"LOAD_VAR    "
            <<idx;

            break;
        }

        case Opcode::JUMP_IF_FALSE:{

            int32_t offset=
            readInt32(bytecode,ip);

            std::cout
            <<"JUMP_IF_FALSE "
            <<offset;

            break;
        }

        case Opcode::JUMP:{

            int32_t offset=
            readInt32(bytecode,ip);

            int target = ip + offset;

            std::cout
            <<"JUMP -> "
            <<target;

            break;
        }

        case Opcode::PRINT:
            std::cout<<"PRINT";
            break;

        case Opcode::INPUT:
            std::cout<<"INPUT";
            break;

        case Opcode::POP:
            std::cout<<"POP";
            break;

        case Opcode::HALT:
            std::cout<<"HALT";
            break;
        }

        std::cout<<std::endl;
    }

    std::cout
    <<"-----------------------\n";
}