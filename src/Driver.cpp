#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Driver.h"
#include "Error.h"
#include "Interpreter.h"
#include "Parser.h"
#include "Resolver.h"
#include "Scanner.h"
#include "Statement.h"
#include "Token.h"

void Driver::runFile( const std::string& path )
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

    Driver::run( buf.str() );

    // Indicate an error in the exit code
    if ( Error::hadError )
        std::exit( 65 );
    if ( Error::hadRuntimeError )
        std::exit( 70 );
}

void Driver::runPrompt()
{
    while ( true )
    {
        std::cout << "> ";

        std::string line{};
        std::getline( std::cin, line );

        // If Control-D then exit
        if ( !std::cin )
            break;

        Driver::run( line );
        Error::hadError = false;
    }
}

void Driver::run( const std::string& source )
{
    Scanner scanner{ source };
    std::vector<Token> tokens{ scanner.scanTokens() };

    Parser parser{ tokens };
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

    if ( Error::hadError )
        return;

    Resolver resolver{ interpreter };
    resolver.resolve( statements );

    if ( Error::hadError )
        return;

    Driver::interpreter.interpret( statements );
}
