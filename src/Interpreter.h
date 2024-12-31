#pragma once
#include <string>

namespace Interpreter
{
    void runFile(const std::string& path);
    void runPrompt();
    void run(const std::string& source);
}
