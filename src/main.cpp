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

        // REPL mode

        if(argc==1){

            std::cout
            <<"===== CVM REPL =====\n";

            std::cout
            <<"Type exit to quit\n\n";

            while(true){

                std::cout
                <<"CVM> ";

                std::string source;

                std::getline(
                std::cin,
                source
                );

                if(source=="exit"){

                    break;
                }

                if(source.empty()){

                    continue;
                }

                try{

                    Lexer lexer(source);

                    auto tokens=
                    lexer.tokenize();

                    Parser parser(tokens);

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

        // Old script mode

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

        buffer<<file.rdbuf();

        std::string source=
        buffer.str();

        Lexer lexer(source);

        auto tokens=
        lexer.tokenize();

        Parser parser(tokens);

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