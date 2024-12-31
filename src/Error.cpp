#include <iostream>
#include <string>

#include "Error.h"

void Error::error(int line, const std::string& message)
{
    Error::report(line, "", message);
}

void Error::report(int line, const std::string& where, const std::string& message)
{
    std::cout << "[line " << line << "] Error" << where << ": " << message << '\n';
    Error::hadError = true;
}