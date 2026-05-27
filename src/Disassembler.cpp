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
static_cast<int64_t>(
bytecode[ip++]
)
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
static_cast<int32_t>(
bytecode[ip++]
)
<<(i*8);

}

return val;

}

void Disassembler::print(

const std::vector<uint8_t>& bytecode,

const std::unordered_map<
uint8_t,
std::string
>& variables

){

size_t ip=0;

std::cout
<<"\n===== DISASSEMBLY =====\n";

while(ip<bytecode.size()){

size_t current=ip;

Opcode op=
static_cast<Opcode>(
bytecode[ip++]
);

std::cout
<<std::right
<<std::setw(4)
<<std::setfill('0')
<<current
<<"   ";

std::cout
<<std::left
<<std::setfill(' ');

switch(op){

case Opcode::PUSH_INT:{

int64_t val=
readInt64(bytecode,ip);

std::cout
<<std::left
<<std::setw(18)
<<"PUSH_INT"
<<val
<<"   [push1]";

break;
}

case Opcode::PUSH_BOOL:{

bool val=
bytecode[ip++];

std::cout
<<std::left
<<std::setw(18)
<<"PUSH_BOOL"
<<(val?"true":"false")
<<"   [push1]";

break;
}

case Opcode::STORE_VAR:{

int idx=
bytecode[ip++];

std::string name=
variables.count(idx)
?
variables.at(idx)
:
"?";

std::cout
<<std::left
<<std::setw(18)
<<"STORE_VAR"
<<name
<<"("
<<idx
<<")"
<<"   [keep stack]";

break;
}

case Opcode::LOAD_VAR:{

int idx=
bytecode[ip++];

std::string name=
variables.count(idx)
?
variables.at(idx)
:
"?";

std::cout
<<std::left
<<std::setw(18)
<<"LOAD_VAR"
<<name
<<"("
<<idx
<<")"
<<"   [push1]";

break;
}

case Opcode::ADD:

std::cout
<<std::setw(18)
<<"ADD"
<<"[pop2->push1]";

break;

case Opcode::SUB:

std::cout
<<std::setw(18)
<<"SUB"
<<"[pop2->push1]";

break;

case Opcode::MUL:

std::cout
<<std::setw(18)
<<"MUL"
<<"[pop2->push1]";

break;

case Opcode::DIV:

std::cout
<<std::setw(18)
<<"DIV"
<<"[pop2->push1]";

break;

case Opcode::EQ:

std::cout
<<std::setw(18)
<<"EQ"
<<"[pop2->push1]";

break;

case Opcode::LT:

std::cout
<<std::setw(18)
<<"LT"
<<"[pop2->push1]";

break;

case Opcode::JUMP_IF_FALSE:{

int32_t offset=
readInt32(bytecode,ip);

int target=
ip+offset;

std::cout
<<std::setw(18)
<<"JUMP_IF_FALSE"
<<"offset("
<<(offset>=0?"+":"")
<<offset
<<") -> "
<<target;

break;
}

case Opcode::JUMP:{

int32_t offset=
readInt32(bytecode,ip);

int target=
ip+offset;

std::cout
<<std::setw(18)
<<"JUMP"
<<"offset("
<<(offset>=0?"+":"")
<<offset
<<") -> "
<<target;

break;
}

case Opcode::PRINT:

std::cout
<<std::setw(18)
<<"PRINT"
<<"[pop1]";

break;

case Opcode::INPUT:

std::cout
<<std::setw(18)
<<"INPUT"
<<"[push1]";

break;

case Opcode::POP:

std::cout
<<std::setw(18)
<<"POP"
<<"[pop1]";

break;

case Opcode::HALT:

std::cout
<<"HALT";

break;
}

std::cout<<std::endl;

}

std::cout
<<"=======================\n";
}