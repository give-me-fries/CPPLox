#include <iostream>
#include <string>

#include "Error.h"
#include "Token.h"

void Error::error( int line, const std::string& message )
{
    Error::report( line, "", message );
}

void Error::error( Token token, const std::string& message )
{
    if ( token.getType() == TokenType::LOX_EOF )
    {
        report( token.getLine(), " at end", message );
    }
    else
    {
        report( token.getLine(), " at '" + token.getLexeme() + "'", message );
    }
}

void Error::report( int line, const std::string& where,
                    const std::string& message )
{
    std::cout << "[line " << line << "] Error" << where << ": " << message
              << '\n';
    Error::hadError = true;
}