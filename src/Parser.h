#pragma once
#include "Token.h"
#include "AST.h"
#include <vector>
#include <stdexcept>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::vector<std::unique_ptr<Stmt>> parse();

private:
    std::vector<Token> tokens;
    size_t current = 0;

    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> letDecl();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> block();
    std::unique_ptr<Stmt> expressionStatement();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> primary();

    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type) const;
    bool isAtEnd() const;
    Token advance();
    Token peek() const;
    Token peekNext() const;
    Token previous() const;
    Token consume(TokenType type, const std::string& message);
};
