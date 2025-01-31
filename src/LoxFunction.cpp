#include <memory>
#include <variant>
#include <vector>

#include "Environment.h"
#include "Interpreter.h"
#include "LoxFunction.h"
#include "Object.h"
#include "ReturnValue.h"

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
        return returnValue.value;
    }

    return Object{ std::monostate{} };
}

std::string LoxFunction::toString() const
{
    return "<fn " + declaration->name.getLexeme() + ">";
}
