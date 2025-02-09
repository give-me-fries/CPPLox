#include <string>

#include "Error.h"
#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "Token.h"

Object LoxInstance::get( const Token& name )
{
    if ( m_fields.find( name.getLexeme() ) != m_fields.end() )
    {
        return m_fields.at( name.getLexeme() );
    }

    std::shared_ptr<LoxFunction> method =
        m_klass->findMethod( name.getLexeme() );
    if ( method.get() )
        return method->bind( shared_from_this() );

    throw Error::RuntimeError{ name, "Undefined property '" + name.getLexeme() +
                                         "'." };
}

void LoxInstance::set( const Token& name, const Object& value )
{
    m_fields.emplace( name.getLexeme(), value );
}

std::string LoxInstance::toString() const
{
    return m_klass->toString() + " instance";
}