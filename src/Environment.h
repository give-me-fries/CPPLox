#pragma once
#include <map>
#include <memory>
#include <string>

#include "Object.h"
#include "Token.h"

class Environment
{
public:
    Environment()
    {
        m_enclosing = nullptr;
    }

    Environment( std::shared_ptr<Environment> enclosing )
        : m_enclosing{ enclosing }
    {
    }

    void define( const std::string& name, const Object& value );
    Object get( const Token& name );
    void assign( const Token& name, const Object& value );

private:
    std::shared_ptr<Environment> m_enclosing = nullptr;
    std::map<std::string, Object> m_values{};
};
