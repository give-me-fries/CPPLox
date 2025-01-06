#pragma once
#include <initializer_list>
#include <variant>

#include "Expression.h"
#include "Token.h"

struct ASTPrinter : public IVisitor
{
private:
    std::string m_tree{};

    void parenthesize( const std::string& name,
                       std::initializer_list<Expr*> exprs )
    {
        m_tree += '(' + name;
        for ( auto expr : exprs )
        {
            m_tree += ' ';
            expr->accept( this );
        }
        m_tree += ")";
    }

public:
    ASTPrinter() = default;

    void print( Expr* expr )
    {
        expr->accept( this );
    }

    void visit( Binary* expr )
    {
        parenthesize( expr->op.getLexeme(),
                      { expr->left.get(), expr->right.get() } );
    }

    void visit( Grouping* expr )
    {
        parenthesize( "group", { expr->expr.get() } );
    }

    void visit( Literal* expr )
    {
        m_tree += objectToString( expr->value );
    }

    void visit( Unary* expr )
    {
        parenthesize( expr->op.getLexeme(), { expr->right.get() } );
    }

    const std::string& getTree() const
    {
        return m_tree;
    }
};
