#include <memory>

#include "Environment.h"
#include "Error.h"
#include "Object.h"

void Environment::define( const std::string& name, const Object& value )
{
    m_values[name] = value;
}

Environment* Environment::ancestor( int distance )
{
    Environment* environment = this;
    for ( int i = 0; i < distance; ++i )
    {
        environment = environment->m_enclosing.get();
    }

    return environment;
}

Object Environment::getAt( int distance, const std::string& name )
{
    return ancestor( distance )->m_values[name];
}

void Environment::assignAt( int distance, const Token& name,
                            const Object& value )
{
    ancestor( distance )->m_values[name.getLexeme()] = value;
}

Object Environment::get( const Token& name )
{
    if ( m_values.find( name.getLexeme() ) != m_values.end() )
    {
        return m_values[name.getLexeme()];
    }

    if ( m_enclosing )
    {
        return m_enclosing->get( name );
    }

    throw Error::RuntimeError{ name, "Undefined variable '" + name.getLexeme() +
                                         "'." };
}

void Environment::assign( const Token& name, const Object& value )
{
    if ( m_values.find( name.getLexeme() ) != m_values.end() )
    {
        m_values[name.getLexeme()] = value;
        return;
    }

    if ( m_enclosing )
    {
        m_enclosing->assign( name, value );
        return;
    }

    throw Error::RuntimeError( name, "Undefined variable '" + name.getLexeme() +
                                         "'." );
}