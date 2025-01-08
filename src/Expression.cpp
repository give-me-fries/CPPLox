#include "Expression.h"

void Binary::accept( IVisitor* visitor )
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

void Unary::accept( IVisitor* visitor )
{
    visitor->visit( this );
}
