#include <cstdlib>
#include <iostream>

#include "Interpreter.h"

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
        Interpreter::runPrompt();
    }

    return 0;
}
