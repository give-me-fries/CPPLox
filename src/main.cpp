#include <cstdlib>
#include <iostream>

#include "Driver.h"

int main( int argc, char** argv )
{
    if ( argc > 2 )
    {
        std::cout << "Usage: cpplox [script]\n";
        std::exit( 64 );
    }
    else if ( argc == 2 )
    {
        Driver::runFile( argv[1] );
    }
    else
    {
        Driver::runPrompt();
    }

    return 0;
}
