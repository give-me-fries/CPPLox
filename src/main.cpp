#include <cstdlib>
#include <iostream>
#include <variant>

#include "Interpreter.h"

#include "ASTPrinter.h"
#include "Expression.h"
#include "Token.h"

int main( int argc, char** argv )
{
    if ( argc > 2 )
    {
        std::cout << "Usage: cpplox [script]\n";
        std::exit( 64 );
    }
    else if ( argc == 2 )
    {
        Interpreter::runFile( argv[1] );
    }
    else
    {
        ASTPrinter printer{};

        Binary expression{
            std::make_unique<Unary>(
                Token{ TokenType::MINUS, "-", std::monostate{}, 1 },
                std::make_unique<Literal>( Object{ 123.0 } ) ),
            Token{ TokenType::STAR, "*", std::monostate{}, 1 },
            std::make_unique<Grouping>(
                std::make_unique<Literal>( Object{ 45.67 } ) ) };

        printer.print( &expression );
        std::cout << printer.getTree() << '\n';
    }

    return 0;
}
