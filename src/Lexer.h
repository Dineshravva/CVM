#pragma once
#include "Token.h"
#include <string>
#include <vector>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t start;
    size_t current;
    int line;

    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    void skipWhitespace();
    Token makeToken(TokenType type);
    Token errorToken(const std::string& message);
    Token identifierOrKeyword();
    Token number();
};
