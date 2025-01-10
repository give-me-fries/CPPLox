#include <string>
#include <variant>

#include "Error.h"
#include "Scanner.h"
#include "Token.h"

using namespace TokenType;

const std::map<std::string, TokenType::Type> Scanner::keywords{
    { "and", AND },     { "class", CLASS },   { "else", ELSE },
    { "false", FALSE }, { "for", FOR },       { "fun", FUN },
    { "if", IF },       { "nil", NIL },       { "or", OR },
    { "print", PRINT }, { "return", RETURN }, { "super", SUPER },
    { "this", THIS },   { "true", TRUE },     { "var", VAR },
    { "while", WHILE } };

bool Scanner::isAtEnd() const
{
    return m_current >= static_cast<int>( m_source.size() );
}

void Scanner::scanToken()
{
    using namespace TokenType;

    // "c" is the current character, advance moves m_current to the next
    // character
    char c = advance();

    switch ( c )
    {
    case '(':
        addToken( LEFT_PAREN );
        break;
    case ')':
        addToken( RIGHT_PAREN );
        break;
    case '{':
        addToken( LEFT_BRACE );
        break;
    case '}':
        addToken( RIGHT_BRACE );
        break;
    case ',':
        addToken( COMMA );
        break;
    case '.':
        addToken( DOT );
        break;
    case '-':
        addToken( MINUS );
        break;
    case '+':
        addToken( PLUS );
        break;
    case ';':
        addToken( SEMICOLON );
        break;
    case '*':
        addToken( STAR );
        break;

    case '!':
        addToken( match( '=' ) ? BANG_EQUAL : BANG );
        break;
    case '=':
        addToken( match( '=' ) ? EQUAL_EQUAL : EQUAL );
        break;
    case '<':
        addToken( match( '=' ) ? LESS_EQUAL : LESS );
        break;
    case '>':
        addToken( match( '=' ) ? GREATER_EQUAL : GREATER );
        break;
    case '/':
        if ( match( '/' ) )
        {
            // A comment goes until the end of the line.
            while ( peek() != '\n' && !isAtEnd() )
                advance();
        }
        else
        {
            addToken( SLASH );
        }
        break;
        // Ignore whitespace.
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        m_line++;
        break;
    case '"':
        string();
        break;
    default:
        if ( isDigit( c ) )
        {
            number();
        }
        else if ( isAlpha( c ) )
        {
            identifier();
        }
        else
        {
            Error::error( m_line, "Unexpected character." );
        }
        break;
    }
}

char Scanner::advance()
{
    return m_source.at( m_current++ );
}

char Scanner::peek() const
{
    if ( isAtEnd() )
        return '\0';
    return m_source.at( m_current );
}

bool Scanner::match( char expected )
{
    if ( isAtEnd() )
        return false;
    if ( m_source.at( m_current ) != expected )
        return false;

    m_current++;
    return true;
}

void Scanner::string()
{
    while ( peek() != '"' && !isAtEnd() )
    {
        if ( peek() == '\n' )
            m_line++;
        advance();
    }

    if ( isAtEnd() )
    {
        Error::error( m_line, "Unterminated string." );
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value = m_source.substr( m_start + 1, m_current - m_start - 2 );
    std::cout << value << '\n';
    addToken( TokenType::STRING, value );
}

bool Scanner::isDigit( char c ) const
{
    return c >= '0' && c <= '9';
}

void Scanner::number()
{
    while ( isDigit( peek() ) )
        advance();

    // Look for a fractional part.
    if ( peek() == '.' && isDigit( peekNext() ) )
    {
        // Consume the ".".
        advance();

        while ( isDigit( ( peek() ) ) )
            advance();
    }

    addToken( TokenType::NUMBER,
              std::stod( m_source.substr( m_start, m_current - m_start ) ) );
}

char Scanner::peekNext() const
{
    if ( m_current + 1 >= static_cast<int>( m_source.size() ) )
        return '\0';
    return m_source.at( m_current + 1 );
}

void Scanner::identifier()
{
    while ( isAlphaNumeric( peek() ) )
        advance();

    std::string text = m_source.substr( m_start, m_current - m_start );
    TokenType::Type type = TokenType::IDENTIFIER;
    if ( keywords.find( text ) != keywords.end() )
        type = keywords.at( text );
    addToken( type );
}

bool Scanner::isAlpha( char c ) const
{
    return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || c == '_';
}

bool Scanner::isAlphaNumeric( char c ) const
{
    return isAlpha( c ) || isDigit( c );
}

void Scanner::addToken( TokenType::Type type )
{
    addToken( type, std::monostate{} );
}

void Scanner::addToken( TokenType::Type type, Object literal )
{
    std::string text = m_source.substr( m_start, m_current - m_start );
    m_tokens.push_back( Token{ type, text, literal, m_line } );
}

std::vector<Token> Scanner::scanTokens()
{
    while ( !isAtEnd() )
    {
        // Beginning of next lexeme.
        m_start = m_current;
        scanToken();
    }

    m_tokens.push_back(
        Token{ TokenType::LOX_EOF, "", std::monostate{}, m_line } );
    return m_tokens;
}