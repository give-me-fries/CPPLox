#include <iostream>
#include <string>
#include <variant>

#include "Token.h"

std::string TokenType::getType(Type type)
{
    return TYPES[type];
}

std::ostream& operator<<(std::ostream& out, const Token& token)
{
    out << token.toString();
    return out;
}

std::string Token::toString() const
{
    return std::string{ TokenType::getType(m_type) + " " + m_lexeme + " " + literalToString() };
}

std::string Token::literalToString() const
{
    if (m_literal.index() == 0)
        return "null";
    else if (m_literal.index() == 1)
        return std::get<std::string>(m_literal);
    else
        return std::to_string(std::get<double>(m_literal));
}