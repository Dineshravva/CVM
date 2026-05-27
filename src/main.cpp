#include "Lexer.h"
#include "Parser.h"
#include "Compiler.h"
#include "VM.h"
#include "Disassembler.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: cvm <script.cvm>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    try {
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        
        Parser parser(tokens);
        auto stmts = parser.parse();

        Compiler compiler;
        std::vector<uint8_t> bytecode = compiler.compile(stmts);

        // Print bytecode
        // std::cout << "--- Bytecode ---" << std::endl;
        // for (size_t i = 0; i < bytecode.size(); i++) {
        //     std::cout << std::hex << (int)bytecode[i] << " ";
        // }
        // std::cout << std::dec << "\n----------------\n" << std::endl;
        

        //new bytecode better
        Disassembler::print(bytecode);

        VM vm;
        vm.execute(bytecode);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
