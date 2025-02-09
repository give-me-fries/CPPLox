#pragma once

#include <map>
#include <memory>
#include <string>

#include "Object.h"

class LoxClass;
class Token;

class LoxInstance : public std::enable_shared_from_this<LoxInstance>
{
public:
    LoxInstance( LoxClass* klass ) : m_klass{ klass }
    {
    }

    Object get( const Token& name );
    void set( const Token& name, const Object& value );
    std::string toString() const;

private:
    LoxClass* m_klass;
    std::map<std::string, Object> m_fields;
};
