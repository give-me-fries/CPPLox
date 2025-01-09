#pragma once
#include <initializer_list>
#include <string>

#include "Expression.h"
#include "Token.h"

struct ASTPrinter : public IVisitor
{
private:
    std::string m_tree{};

    void parenthesize( const std::string& name,
                       std::initializer_list<Expr*> exprs );

public:
    void print( Expr* expr );
    void visit( Binary* expr );
    void visit( Grouping* expr );
    void visit( Literal* expr );
    void visit( Unary* expr );
    const std::string& getTree() const;
};
