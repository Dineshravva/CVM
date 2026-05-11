#pragma once
#include <string>

enum class TokenType {
    INT_LITERAL, BOOL_LITERAL, IDENTIFIER,
    PLUS, MINUS, STAR, SLASH,
    EQ_EQ, LESS, EQUAL,
    LET, IF, ELSE, WHILE, PRINT, INPUT,
    LPAREN, RPAREN, LBRACE, RBRACE, SEMICOLON,
    END_OF_FILE, ERROR_TOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};
