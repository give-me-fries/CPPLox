#pragma once
#include <string>

#include "Interpreter.h"

namespace Driver
{
    void runFile( const std::string& path );
    void runPrompt();
    void run( const std::string& source );
    static Interpreter interpreter{};
} // namespace Driver
