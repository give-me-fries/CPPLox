#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Error.h"
#include "Interpreter.h"
#include "Scanner.h"
#include "Token.h"

void Interpreter::runFile( const std::string& path )
{
    std::ifstream infile{ path };

    if ( !infile )
    {
        std::cout << "Error: invalid file path\n";
        std::exit( 64 );
    }

    std::stringstream buf{};
    buf << infile.rdbuf();
    infile.close();

    Interpreter::run( buf.str() );

    // Indicate an error in the exit code/
    if ( Error::hadError )
        std::exit( 65 );
}

void Interpreter::runPrompt()
{
    while ( true )
    {
        std::cout << "> ";

        std::string line{};
        std::getline( std::cin, line );

        // If Control-D then exit
        if ( !std::cin )
            break;

        Interpreter::run( line );
        Error::hadError = false;
    }
}

void Interpreter::run( const std::string& source )
{
    Scanner scanner{ source };
    std::vector<Token> tokens{ scanner.scanTokens() };

    for ( const auto& token : tokens )
    {
        std::cout << token << '\n';
    }
}
