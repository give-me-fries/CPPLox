#pragma once
#include <string>
#include <variant>

#include "LoxCallable.h"

class LoxCallable;

using Object = std::variant<std::monostate, std::string, double, bool,
                            std::shared_ptr<LoxCallable>>;

std::string objectToString( const Object& obj );
