#pragma once
#include <exception>

#include "Object.h"

class ReturnValue : std::exception
{
public:
    ReturnValue( const Object& value ) : value{ value }
    {
    }

    Object value;
};