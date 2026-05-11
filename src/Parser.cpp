#include "Parser.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::declaration() {
    if (match({TokenType::LET})) return letDecl();
    return statement();
}

std::unique_ptr<Stmt> Parser::letDecl() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    consume(TokenType::EQUAL, "Expect '=' after variable name.");
    std::unique_ptr<Expr> initializer = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<LetStmt>(std::move(name), std::move(initializer));
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::LBRACE})) return block();
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LPAREN, "Expect '(' after 'if'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RPAREN, "Expect ')' after if condition.");
    
    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }
    
    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LPAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RPAREN, "Expect ')' after condition.");
    std::unique_ptr<Stmt> body = statement();
    
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(TokenType::RBRACE, "Expect '}' after block.");
    return std::make_unique<BlockStmt>(std::move(statements));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    if (check(TokenType::IDENTIFIER) && peekNext().type == TokenType::EQUAL) {
        Token name = advance();
        advance(); // consume '='
        std::unique_ptr<Expr> value = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after assignment.");
        return std::make_unique<AssignStmt>(std::move(name), std::move(value));
    }
    
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<ExprStmt>(std::move(expr));
}

std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();
    while (match({TokenType::EQ_EQ})) {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(op), std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();
    while (match({TokenType::LESS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(op), std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(op), std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = primary();
    while (match({TokenType::STAR, TokenType::SLASH})) {
        Token op = previous();
        std::unique_ptr<Expr> right = primary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(op), std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::INT_LITERAL, TokenType::BOOL_LITERAL})) {
        return std::make_unique<LiteralExpr>(previous());
    }
    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<VariableExpr>(previous());
    }
    if (match({TokenType::INPUT})) {
        return std::make_unique<InputExpr>();
    }
    if (match({TokenType::LPAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RPAREN, "Expect ')' after expression.");
        return expr;
    }
    throw std::runtime_error("Expect expression at line " + std::to_string(peek().line) + ": " + peek().lexeme);
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::peekNext() const {
    if (current + 1 >= tokens.size()) return tokens.back();
    return tokens[current + 1];
}

Token Parser::previous() const {
    if (current == 0) return tokens[0];
    return tokens[current - 1];
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error("Line " + std::to_string(peek().line) + ": " + message);
}
