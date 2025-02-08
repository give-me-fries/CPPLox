#include <memory>
#include <variant>
#include <vector>

#include "Environment.h"
#include "Interpreter.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "Object.h"
#include "ReturnValue.h"

std::shared_ptr<LoxFunction> LoxFunction::bind(
    std::shared_ptr<LoxInstance> instance )
{
    std::shared_ptr<Environment> environment =
        std::make_shared<Environment>( Environment{ closure } );
    environment->define( "this", instance );
    return std::make_shared<LoxFunction>( declaration, environment,
                                          m_isInitializer );
}

int LoxFunction::arity() const
{
    return static_cast<int>( declaration->params.size() );
}

Object LoxFunction::call( Interpreter& interpreter,
                          const std::vector<Object>& arguments )
{
    std::shared_ptr<Environment> environment{ new Environment{ closure } };
    for ( std::size_t i = 0; i < declaration->params.size(); ++i )
    {
        environment->define( declaration->params.at( i ).getLexeme(),
                             arguments.at( i ) );
    }

    try
    {
        interpreter.executeBlock( declaration->body, environment );
    }
    catch ( const ReturnValue& returnValue )
    {
        if ( m_isInitializer )
            closure->getAt( 0, "this" );

        return returnValue.value;
    }

    if ( m_isInitializer )
        return closure->getAt( 0, "this" );

    return Object{ std::monostate{} };
}

std::string LoxFunction::toString() const
{
    return "<fn " + declaration->name.getLexeme() + ">";
}
