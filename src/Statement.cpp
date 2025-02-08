#include "Statement.h"
#include "Visitor.h"

void Block::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void ClassStmt::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Expression::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Function::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void If::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Print::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Return::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Var::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void While::accept( IVisitor* visitor )
{
    visitor->visit( this );
}