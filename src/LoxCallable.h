#pragma once
#include <memory>
#include <string>
#include <variant>
#include <vector>

class Interpreter;

class LoxCallable
{
public:
    virtual int arity() const = 0;
    virtual std::variant<std::monostate, std::string, double, bool,
                         std::shared_ptr<LoxCallable>>
    call( Interpreter& interpreter,
          const std::vector<std::variant<std::monostate, std::string, double,
                                         bool, std::shared_ptr<LoxCallable>>>&
              arguments ) = 0;
    virtual std::string toString() const = 0;
    virtual ~LoxCallable() = default;
};