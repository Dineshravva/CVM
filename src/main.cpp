#include "Lexer.h"
#include "Parser.h"
#include "Compiler.h"
#include "VM.h"
#include "Disassembler.h"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc,char* argv[])
{
    Compiler compiler;
    VM vm;

    try{

        // REPL MODE

        if(argc==1){

            std::cout
            <<"===== CVM REPL =====\n";

            std::cout
            <<"Type exit to quit\n\n";

            while(true){

                std::string source;
                std::string line;

                int openBraces=0;

                std::cout
                <<"CVM> ";

                std::getline(
                std::cin,
                line
                );

                if(line=="exit"){

                    break;
                }

                if(line.empty()){

                    continue;
                }

                source+=line+"\n";

                for(char c:line){

                    if(c=='{')
                        openBraces++;

                    else if(c=='}')
                        openBraces--;
                }

                // multiline mode

                while(openBraces>0){

                    std::cout
                    <<"... ";

                    std::getline(
                    std::cin,
                    line
                    );

                    source+=line+"\n";

                    for(char c:line){

                        if(c=='{')
                            openBraces++;

                        else if(c=='}')
                            openBraces--;
                    }
                }

                try{

                    Lexer lexer(
                    source
                    );

                    auto tokens=
                    lexer.tokenize();

                    Parser parser(
                    tokens
                    );

                    auto stmts=
                    parser.parse();

                    auto bytecode=

                    compiler.compile(
                    stmts
                    );

                    Disassembler::print(

                    bytecode,

                    compiler.getVariableNames()

                    );

                    vm.execute(
                    bytecode
                    );
                }

                catch(
                const std::exception& e
                ){

                    std::cerr

                    <<"Error: "

                    <<e.what()

                    <<"\n";
                }
            }

            return 0;
        }

        // OLD SCRIPT MODE

        if(argc!=2){

            std::cerr
            <<"Usage: cvm <script.cvm>\n";

            return 1;
        }

        std::ifstream file(
        argv[1]
        );

        if(!file.is_open()){

            std::cerr
            <<"Failed to open file: "
            <<argv[1]
            <<"\n";

            return 1;
        }

        std::stringstream buffer;

        buffer
        <<file.rdbuf();

        std::string source=
        buffer.str();

        Lexer lexer(
        source
        );

        auto tokens=
        lexer.tokenize();

        Parser parser(
        tokens
        );

        auto stmts=
        parser.parse();

        auto bytecode=
        compiler.compile(
        stmts
        );

        Disassembler::print(

        bytecode,

        compiler.getVariableNames()

        );

        vm.execute(
        bytecode
        );

    }

    catch(
    const std::exception& e
    ){

        std::cerr
        <<"Error: "
        <<e.what()
        <<"\n";

        return 1;
    }

    return 0;
}