#include <iostream>
#include <string>

#include "Token.h"

std::string TokenType::getType( Type type )
{
    return TYPES[type];
}

std::ostream& operator<<( std::ostream& out, const Token& token )
{
    out << token.toString();
    return out;
}

std::string Token::toString() const
{
    return std::string{ TokenType::getType( m_type ) + " " + m_lexeme + " " +
                        literalToString() };
}

std::string Token::getLexeme() const
{
    return m_lexeme;
}

TokenType::Type Token::getType() const
{
    return m_type;
}

int Token::getLine() const
{
    return m_line;
}

std::string Token::literalToString() const
{
    if ( m_literal.index() == 0 )
        return "null";
    else if ( m_literal.index() == 1 )
        return std::get<std::string>( m_literal );
    else
        return std::to_string( std::get<double>( m_literal ) );
}

std::string objectToString( const Object& obj )
{
    if ( obj.index() == 0 )
        return "nil";
    else if ( obj.index() == 1 )
        return std::get<std::string>( obj );
    else
    {
        std::string res = std::to_string( std::get<double>( obj ) );
        res.erase( res.find_last_not_of( '0' ) + 1, std::string::npos );
        res.erase( res.find_last_not_of( '.' ) + 1, std::string::npos );
        return res;
    }
}