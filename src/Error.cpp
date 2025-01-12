#include <iostream>
#include <string>
#include <string_view>

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

void Error::runtimeError( const RuntimeError& error )
{
    std::cerr << error.what() << "\n[line " << error.getToken().getLine()
              << ']';
    hadRuntimeError = true;
}

void Error::report( int line, const std::string& where,
                    const std::string& message )
{
    std::cout << "[line " << line << "] Error" << where << ": " << message
              << '\n';
    Error::hadError = true;
}

Error::RuntimeError::RuntimeError( Token token, std::string_view error )
    : m_token{ token }, m_error{ error }
{
}

const char* Error::RuntimeError::what() const noexcept
{
    return m_error.c_str();
}

const Token& Error::RuntimeError::getToken() const
{
    return m_token;
}