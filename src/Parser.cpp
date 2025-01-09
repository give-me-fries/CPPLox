#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

#include "Error.h"
#include "Expression.h"
#include "Parser.h"
#include "Token.h"

ParseError::ParseError( const std::string& error ) : std::runtime_error{ error }
{
}

std::unique_ptr<Expr> Parser::expression()
{
    return equality();
}

std::unique_ptr<Expr> Parser::equality()
{
    std::unique_ptr<Expr> expr = comparison();

    while ( match( { TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL } ) )
    {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<Binary>( std::move( expr ), op,
                                         std::move( right ) );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison()
{
    std::unique_ptr<Expr> expr = term();

    while ( match( { TokenType::GREATER, TokenType::GREATER_EQUAL,
                     TokenType::LESS, TokenType::LESS_EQUAL } ) )
    {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<Binary>( std::move( expr ), op,
                                         std::move( right ) );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term()
{
    std::unique_ptr<Expr> expr = factor();

    while ( match( { TokenType::MINUS, TokenType::PLUS } ) )
    {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<Binary>( std::move( expr ), op,
                                         std::move( right ) );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor()
{
    std::unique_ptr<Expr> expr = unary();

    while ( match( { TokenType::SLASH, TokenType::STAR } ) )
    {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<Binary>( std::move( expr ), op,
                                         std::move( right ) );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary()
{
    if ( match( { TokenType::BANG, TokenType::MINUS } ) )
    {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Unary>( op, std::move( right ) );
    }

    return primary();
}

std::unique_ptr<Expr> Parser::primary()
{
    if ( match( { TokenType::FALSE } ) )
        return std::make_unique<Literal>( Object{ "false" } );
    if ( match( { TokenType::TRUE } ) )
        return std::make_unique<Literal>( Object{ "true" } );
    if ( match( { TokenType::NIL } ) )
        return std::make_unique<Literal>( Object{ std::monostate{} } );

    if ( match( { TokenType::NUMBER, TokenType::STRING } ) )
        return std::make_unique<Literal>( Object{ previous().getLexeme() } );

    if ( match( { TokenType::LEFT_PAREN } ) )
    {
        std::unique_ptr<Expr> expr = expression();
        consume( TokenType::RIGHT_PAREN, "Expect ')' after expression." );
        return std::make_unique<Grouping>( std::move( expr ) );
    }

    throw error( peek(), "Expected expression." );
}

bool Parser::match( std::initializer_list<TokenType::Type> types )
{
    for ( const auto& type : types )
    {
        if ( check( type ) )
        {
            advance();
            return true;
        }
    }

    return false;
}

Token& Parser::consume( TokenType::Type type, const std::string& message )
{
    if ( check( type ) )
        return advance();

    throw error( peek(), message );
}

bool Parser::check( TokenType::Type type )
{
    if ( isAtEnd() )
        return false;

    return peek().getType() == type;
}

Token& Parser::advance()
{
    if ( !isAtEnd() )
        m_current++;

    return previous();
}

bool Parser::isAtEnd()
{
    return peek().getType() == TokenType::LOX_EOF;
}

Token& Parser::peek()
{
    return m_tokens.at( m_current );
}

Token& Parser::previous()
{
    return m_tokens.at( m_current - 1 );
}

ParseError Parser::error( Token token, const std::string& message )
{
    Error::error( token, message );
    return ParseError{ "" };
}

void Parser::synchronize()
{
    advance();

    while ( !isAtEnd() )
    {
        if ( previous().getType() == TokenType::SEMICOLON )
            return;

        switch ( peek().getType() )
        {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        default:
            break;
        }

        advance();
    }
}

Parser::Parser( const std::vector<Token>& tokens ) : m_tokens{ tokens }
{
}

std::unique_ptr<Expr> Parser::parse()
{
    try
    {
        return expression();
    }
    catch ( const ParseError& error )
    {
        return nullptr;
    }
}