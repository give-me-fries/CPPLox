#pragma once
#include <string>
#include <vector>

#include "LoxCallable.h"
#include "Object.h"

class LoxClock : public LoxCallable
{
public:
    LoxClock() = default;

    int arity() const override;
    [[maybe_unused]] Object call(
        [[maybe_unused]] Interpreter& interpreter,
        [[maybe_unused]] const std::vector<Object>& arguments ) override;
    std::string toString() const override;
};
