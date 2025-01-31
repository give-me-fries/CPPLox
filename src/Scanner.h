#pragma once
#include <map>
#include <string>
#include <vector>

#include "Object.h"
#include "Token.h"

class Scanner
{
public:
    Scanner( std::string source ) : m_source{ source }
    {
    }

    std::vector<Token> scanTokens();

private:
    bool isAtEnd() const;
    void scanToken();
    char advance();
    char peek() const;
    bool match( char expected );
    void string();
    bool isDigit( char c ) const;
    void number();
    char peekNext() const;
    void identifier();
    bool isAlpha( char c ) const;
    bool isAlphaNumeric( char c ) const;
    void addToken( TokenType::Type type );
    void addToken( TokenType::Type type, Object literal );

    int m_start{ 0 };
    int m_current{ 0 };
    int m_line{ 1 };
    const std::string m_source{};
    std::vector<Token> m_tokens{};

    static const std::map<std::string, TokenType::Type> keywords;
};