#include "Expression.h"
#include "Visitor.h"

void Assign::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Binary::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Call::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Get::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Grouping::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Literal::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Logical::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Set::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Super::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void This::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Unary::accept( IVisitor* visitor )
{
    visitor->visit( this );
}

void Variable::accept( IVisitor* visitor )
{
    visitor->visit( this );
}