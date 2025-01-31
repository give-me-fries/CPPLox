#include <chrono>
#include <string>
#include <vector>

#include "Interpreter.h"
#include "LoxClock.h"
#include "Object.h"

int LoxClock::arity() const
{
    return 0;
}

[[maybe_unused]] Object LoxClock::call(
    [[maybe_unused]] Interpreter& interpreter,
    [[maybe_unused]] const std::vector<Object>& arguments )
{
    using namespace std::chrono;
    auto time =
        duration_cast<milliseconds>( steady_clock::now().time_since_epoch() );
    return Object{ static_cast<double>( time.count() ) };
}

std::string LoxClock::toString() const
{
    return "<native fn>";
}