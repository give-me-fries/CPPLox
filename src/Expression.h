#pragma once
#include <memory>
#include <string>

#include "Token.h"
#include "Visitor.h"

struct Expr
{
    virtual void accept( IVisitor* visitor ) = 0;
    virtual ~Expr()
    {
    }
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

    Literal( const Object value ) : value{ value }
    {
    }

    Object value;
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
