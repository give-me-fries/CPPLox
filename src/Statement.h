#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "Expression.h"
#include "Visitor.h"

struct Stmt
{
    virtual void accept( IVisitor* visitor ) = 0;
    virtual ~Stmt() = default;
};

struct Block : public Stmt
{
    void accept( IVisitor* visitor ) override;

    Block( std::vector<std::unique_ptr<Stmt>> statements )
        : statements{ std::move( statements ) }
    {
    }

    std::vector<std::unique_ptr<Stmt>> statements;
};

struct ClassStmt : public Stmt
{
    void accept( IVisitor* visitor ) override;

    ClassStmt( const Token& name,
               std::vector<std::unique_ptr<Function>> methods )
        : name{ name }, methods{ std::move( methods ) }
    {
    }

    Token name;
    std::vector<std::unique_ptr<Function>> methods;
};

struct Expression : public Stmt
{
    void accept( IVisitor* visitor ) override;

    Expression( std::unique_ptr<Expr> expression )
        : expression{ std::move( expression ) }
    {
    }

    std::unique_ptr<Expr> expression;
};

struct Function : public Stmt
{
    void accept( IVisitor* visitor ) override;

    Function( const Token& name, const std::vector<Token>& params,
              std::vector<std::unique_ptr<Stmt>> body )
        : name{ name }, params{ std::move( params ) }, body{ std::move( body ) }
    {
    }

    Token name;
    std::vector<Token> params;
    std::vector<std::unique_ptr<Stmt>> body;
};

struct If : public Stmt
{
    void accept( IVisitor* visitor ) override;

    If( std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch,
        std::unique_ptr<Stmt> elseBranch )
        : condition{ std::move( condition ) },
          thenBranch{ std::move( thenBranch ) },
          elseBranch{ std::move( elseBranch ) }
    {
    }

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
};

struct Print : public Stmt
{
    void accept( IVisitor* visitor ) override;

    Print( std::unique_ptr<Expr> expression )
        : expression{ std::move( expression ) }
    {
    }

    std::unique_ptr<Expr> expression;
};

struct Return : public Stmt
{
    void accept( IVisitor* visitor ) override;

    Return( const Token& keyword, std::unique_ptr<Expr> value )
        : keyword{ keyword }, value{ std::move( value ) }
    {
    }

    Token keyword;
    std::unique_ptr<Expr> value;
};

struct Var : public Stmt
{
    void accept( IVisitor* visitor ) override;

    Var( const Token& name, std::unique_ptr<Expr> initializer )
        : name{ name }, initializer{ std::move( initializer ) }
    {
    }

    Token name;
    std::unique_ptr<Expr> initializer;
};

struct While : public Stmt
{
    void accept( IVisitor* visitor ) override;

    While( std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body )
        : condition{ std::move( condition ) }, body{ std::move( body ) }
    {
    }

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};
