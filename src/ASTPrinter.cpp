#include <initializer_list>
#include <string>

#include "ASTPrinter.h"
#include "Expression.h"
#include "Token.h"

void ASTPrinter::parenthesize( const std::string& name,
                               std::initializer_list<Expr*> exprs )
{
    m_tree += '(' + name;
    for ( auto expr : exprs )
    {
        m_tree += " ";
        expr->accept( this );
    }
    m_tree += ')';
}

void ASTPrinter::print( Expr* expr )
{
    expr->accept( this );
}

void ASTPrinter::visit( Binary* expr )
{
    std::cout << "Binary\n";
    parenthesize( expr->op.getLexeme(),
                  { expr->left.get(), expr->right.get() } );
}
void ASTPrinter::visit( Grouping* expr )
{
    std::cout << "Grouping\n";
    parenthesize( "group", { expr->expr.get() } );
}

void ASTPrinter::visit( Literal* expr )
{
    std::cout << "Literal\n";
    m_tree += objectToString( expr->value );
}

void ASTPrinter::visit( Unary* expr )
{
    std::cout << "Unary\n";
    parenthesize( expr->op.getLexeme(), { expr->right.get() } );
}

const std::string& ASTPrinter::getTree() const
{
    return m_tree;
}
