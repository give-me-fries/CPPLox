#pragma once
#include <memory>
#include <string>

#include "Token.h"

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct IVisitor
{
    virtual void visit( Binary* ) = 0;
    virtual void visit( Grouping* ) = 0;
    virtual void visit( Literal* ) = 0;
    virtual void visit( Unary* ) = 0;
    virtual ~IVisitor() = default;
};

struct Expr
{
    virtual void accept( IVisitor* visitor ) = 0;
    virtual ~Expr() = default;
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

    Literal( const Object& value ) : value{ value }
    {
    }

    Object value;
};

struct Unary : public Expr
{
    void accept( IVisitor* visitor ) override;

    Unary( const Token& op, std::unique_ptr<Expr> right )
        : op{ op }, right{ std::move( right ) }
    {
    }

    Token op;
    std::unique_ptr<Expr> right;
};
