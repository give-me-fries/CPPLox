#pragma once
#include <string>
#include <variant>

#include "LoxCallable.h"

class LoxCallable;
class LoxInstance;

using Object =
    std::variant<std::monostate, std::string, double, bool,
                 std::shared_ptr<LoxCallable>, std::shared_ptr<LoxInstance>>;

std::string objectToString( const Object& obj );
