#pragma once
#include "Token.h"
#include <memory>
#include <vector>

struct Expr {
    virtual ~Expr() = default;
};

struct LiteralExpr : Expr {
    Token value;
    LiteralExpr(Token value) : value(std::move(value)) {}
};

struct BinaryExpr : Expr {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
};

struct VariableExpr : Expr {
    Token name;
    VariableExpr(Token name) : name(std::move(name)) {}
};

struct InputExpr : Expr {
    InputExpr() {}
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct LetStmt : Stmt {
    Token name;
    std::unique_ptr<Expr> initializer;
    LetStmt(Token name, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}
};

struct AssignStmt : Stmt {
    Token name;
    std::unique_ptr<Expr> value;
    AssignStmt(Token name, std::unique_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)) {}
};

struct PrintStmt : Stmt {
    std::unique_ptr<Expr> expression;
    PrintStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
};

struct ExprStmt : Stmt {
    std::unique_ptr<Expr> expression;
    ExprStmt(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
};

struct BlockStmt : Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {}
};

struct IfStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
};

struct WhileStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}
};
