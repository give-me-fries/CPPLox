#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Interpreter.h"

void Interpreter::runFile(const std::string& path)
{
    std::ifstream infile{path};
    
    if (!infile)
    {
        std::cout << "Error: invalid file path\n";
        std::exit(64);
    }

    std::stringstream buf{};
    buf << infile.rdbuf();
    infile.close();
    
    Interpreter::run(buf.str());
}

void Interpreter::runPrompt()
{
    ;
}

void Interpreter::run(const std::string& source)
{
    ;
}