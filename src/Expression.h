#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Object.h"
#include "Token.h"
#include "Visitor.h"

struct Expr
{
    virtual void accept( IVisitor* visitor ) = 0;
    virtual ~Expr() = default;
};

struct Assign : public Expr
{
    void accept( IVisitor* visitor ) override;

    Assign( const Token& name, std::unique_ptr<Expr> value )
        : name{ name }, value{ std::move( value ) }
    {
    }

    Token name;
    std::unique_ptr<Expr> value;
};

struct Binary : public Expr
{
    void accept( IVisitor* visitor ) override;

    Binary( std::unique_ptr<Expr> left, const Token& op,
            std::unique_ptr<Expr> right )
        : left{ std::move( left ) }, op{ op }, right{ std::move( right ) }
    {
    }

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

struct Call : public Expr
{
    void accept( IVisitor* visitor ) override;

    Call( std::unique_ptr<Expr> callee, const Token& paren,
          std::vector<std::unique_ptr<Expr>> arguments )
        : callee{ std::move( callee ) }, paren{ paren },
          arguments{ std::move( arguments ) }
    {
    }

    std::unique_ptr<Expr> callee;
    Token paren;
    std::vector<std::unique_ptr<Expr>> arguments;
};

struct Grouping : public Expr
{
    void accept( IVisitor* visitor ) override;

    Grouping( std::unique_ptr<Expr> expr ) : expr{ std::move( expr ) }
    {
    }

    std::unique_ptr<Expr> expr;
};

struct Literal : public Expr
{
    void accept( IVisitor* visitor ) override;

    Literal( const Object& value ) : value{ value }
    {
    }

    Object value;
};

struct Logical : public Expr
{
    void accept( IVisitor* visitor ) override;

    Logical( std::unique_ptr<Expr> left, const Token& op,
             std::unique_ptr<Expr> right )
        : left{ std::move( left ) }, op{ op }, right{ std::move( right ) }
    {
    }

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

struct Unary : public Expr
{
    void accept( IVisitor* visitor ) override;

    Unary( const Token op, std::unique_ptr<Expr> right )
        : op{ op }, right{ std::move( right ) }
    {
    }

    Token op;
    std::unique_ptr<Expr> right;
};

struct Variable : public Expr
{
    void accept( IVisitor* visitor ) override;

    Variable( const Token& name ) : name{ name }
    {
    }

    Token name;
};