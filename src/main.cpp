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

        Binary e{
            new Unary{ Token{ TokenType::MINUS, "-", std::monostate{}, 1 },
                       new Literal{ Object{ 123.0 } } },
            Token{ TokenType::STAR, "*", std::monostate{}, 1 },
            new Grouping{ new Literal{ Object{ 45.67 } } } };

        printer.print( &e );
        std::cout << printer.getTree();

        Interpreter::runPrompt();
    }

    return 0;
}
