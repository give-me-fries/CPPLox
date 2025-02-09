#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

#include "Error.h"
#include "Expression.h"
#include "Object.h"
#include "Parser.h"
#include "Statement.h"
#include "Token.h"

ParseError::ParseError( const std::string& error ) : std::runtime_error{ error }
{
}

std::unique_ptr<Expr> Parser::expression()
{
    return assignment();
}

std::unique_ptr<Stmt> Parser::declaration()
{
    try
    {
        if ( match( { TokenType::CLASS } ) )
        {
            return classDeclaration();
        }
        if ( match( { TokenType::FUN } ) )
        {
            return function( "function" );
        }
        if ( match( { TokenType::VAR } ) )
        {
            return varDeclaration();
        }

        return statement();
    }
    catch ( const ParseError& error )
    {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::classDeclaration()
{
    Token name = consume( TokenType::IDENTIFIER, "Expect class name." );

    std::unique_ptr<Variable> superclass = nullptr;
    if ( match( { TokenType::LESS } ) )
    {
        consume( TokenType::IDENTIFIER, "Expect superclass name." );
        superclass = std::make_unique<Variable>( previous() );
    }

    consume( TokenType::LEFT_BRACE, "Expect '{' before class body." );
    std::vector<std::unique_ptr<Function>> methods{};
    while ( !check( TokenType::RIGHT_BRACE ) && !isAtEnd() )
    {
        methods.push_back( std::move( function( "method" ) ) );
    }

    consume( TokenType::RIGHT_BRACE, "Expect '}' after class body." );

    return std::make_unique<ClassStmt>( name, std::move( superclass ),
                                        std::move( methods ) );
}

std::unique_ptr<Stmt> Parser::statement()
{
    if ( match( { TokenType::FOR } ) )
    {
        return forStatement();
    }

    if ( match( { TokenType::IF } ) )
    {
        return ifStatement();
    }

    if ( match( { TokenType::PRINT } ) )
    {
        return printStatement();
    }

    if ( match( { TokenType::RETURN } ) )
    {
        return returnStatement();
    }

    if ( match( { TokenType::WHILE } ) )
    {
        return whileStatement();
    }

    if ( match( { TokenType::LEFT_BRACE } ) )
    {
        return std::make_unique<Block>( block() );
    }

    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::forStatement()
{
    consume( TokenType::LEFT_PAREN, "Expect '(' after 'for'." );

    std::unique_ptr<Stmt> initializer;
    if ( match( { TokenType::SEMICOLON } ) )
    {
        initializer = nullptr;
    }
    else if ( match( { TokenType::VAR } ) )
    {
        initializer = varDeclaration();
    }
    else
    {
        initializer = expressionStatement();
    }

    std::unique_ptr<Expr> condition = nullptr;
    if ( !check( TokenType::SEMICOLON ) )
    {
        condition = expression();
    }

    consume( TokenType::SEMICOLON, "Expect ';' after loop condition." );

    std::unique_ptr<Expr> increment = nullptr;
    if ( !check( TokenType::RIGHT_PAREN ) )
    {
        increment = expression();
    }
    consume( TokenType::RIGHT_PAREN, "Expect ')' after for clauses." );

    std::unique_ptr<Stmt> body = statement();

    if ( increment )
    {
        std::vector<std::unique_ptr<Stmt>> temp;
        temp.push_back( std::move( body ) );
        temp.push_back(
            std::make_unique<Expression>( std::move( increment ) ) );
        body = std::make_unique<Block>( std::move( temp ) );
    }

    if ( !condition )
    {
        condition = std::make_unique<Literal>( true );
    }
    body = std::make_unique<While>( std::move( condition ), std::move( body ) );

    if ( initializer )
    {
        std::vector<std::unique_ptr<Stmt>> temp;
        temp.push_back( std::move( initializer ) );
        temp.push_back( std::move( body ) );
        body = std::make_unique<Block>( std::move( temp ) );
    }

    return body;
}

std::unique_ptr<Stmt> Parser::ifStatement()
{
    consume( TokenType::LEFT_PAREN, "Expect '(' after 'if'." );
    std::unique_ptr<Expr> condition = expression();
    consume( TokenType::RIGHT_PAREN, "Expect ')' after if condition." );

    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if ( match( { TokenType::ELSE } ) )
    {
        elseBranch = statement();
    }

    return std::make_unique<If>( std::move( condition ),
                                 std::move( thenBranch ),
                                 std::move( elseBranch ) );
}

std::unique_ptr<Stmt> Parser::printStatement()
{
    std::unique_ptr<Expr> value = expression();
    consume( TokenType::SEMICOLON, "Expect ';' after value." );
    return std::make_unique<Print>( std::move( value ) );
}

std::unique_ptr<Stmt> Parser::returnStatement()
{
    Token keyword = previous();
    std::unique_ptr<Expr> value = nullptr;
    if ( !check( TokenType::SEMICOLON ) )
    {
        value = expression();
    }

    consume( TokenType::SEMICOLON, "Expect ';' after return value." );
    return std::make_unique<Return>( keyword, std::move( value ) );
}

std::unique_ptr<Stmt> Parser::whileStatement()
{
    consume( TokenType::LEFT_PAREN, "Expect '(' after 'while'." );
    std::unique_ptr<Expr> condition = expression();
    consume( TokenType::RIGHT_PAREN, "Expect ')' after condition." );
    std::unique_ptr<Stmt> body = statement();

    return std::make_unique<While>( std::move( condition ), std::move( body ) );
}

std::unique_ptr<Stmt> Parser::varDeclaration()
{
    Token name = consume( TokenType::IDENTIFIER, "Expect variable name." );

    std::unique_ptr<Expr> initializer = nullptr;
    if ( match( { TokenType::EQUAL } ) )
    {
        initializer = expression();
    }

    consume( TokenType::SEMICOLON, "Expect ';' after variable declaration." );
    return std::make_unique<Var>( name, std::move( initializer ) );
}

std::unique_ptr<Stmt> Parser::expressionStatement()
{
    std::unique_ptr<Expr> expr = expression();
    consume( TokenType::SEMICOLON, "Expect ';' after expresison." );
    return std::make_unique<Expression>( std::move( expr ) );
}

std::unique_ptr<Function> Parser::function( const std::string& kind )
{
    Token name = consume( TokenType::IDENTIFIER, "Expect " + kind + " name." );

    consume( TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name." );
    std::vector<Token> parameters{};

    if ( !check( TokenType::RIGHT_PAREN ) )
    {
        do
        {
            if ( parameters.size() >= 255 )
            {
                Error::error( peek(), "Can't have more than 255 parameters." );
            }

            parameters.push_back(
                consume( TokenType::IDENTIFIER, "Expect parameter name." ) );

        } while ( match( { TokenType::COMMA } ) );
    }

    consume( TokenType::RIGHT_PAREN, "Expect ')' after parameters." );

    consume( TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body." );

    std::vector<std::unique_ptr<Stmt>> body{ std::move( block() ) };

    return std::make_unique<Function>( name, std::move( parameters ),
                                       std::move( body ) );
}

std::vector<std::unique_ptr<Stmt>> Parser::block()
{
    std::vector<std::unique_ptr<Stmt>> statements{};

    while ( !check( TokenType::RIGHT_BRACE ) && !isAtEnd() )
    {
        statements.push_back( std::move( declaration() ) );
    }

    consume( TokenType::RIGHT_BRACE, "Expect '}' after block." );
    return statements;
}

std::unique_ptr<Expr> Parser::assignment()
{
    std::unique_ptr<Expr> expr = orExpression();

    if ( match( { TokenType::EQUAL } ) )
    {
        Token equals = previous();
        std::unique_ptr<Expr> value = assignment();

        if ( dynamic_cast<Variable*>( expr.get() ) )
        {
            Token name = dynamic_cast<Variable*>( expr.get() )->name;
            return std::make_unique<Assign>( name, std::move( value ) );
        }
        else if ( dynamic_cast<Get*>( expr.get() ) )
        {
            Token name = dynamic_cast<Get*>( expr.get() )->name;
            std::unique_ptr<Expr> object =
                std::move( dynamic_cast<Get*>( expr.get() )->object );
            return std::make_unique<Set>( std::move( object ), name,
                                          std::move( value ) );
        }

        Error::error( equals, "Invalid assignment target." );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::orExpression()
{
    std::unique_ptr<Expr> expr = andExpression();

    while ( match( { TokenType::OR } ) )
    {
        Token op = previous();
        std::unique_ptr<Expr> right = andExpression();
        expr = std::make_unique<Logical>( std::move( expr ), op,
                                          std::move( right ) );
    }

    return expr;
}

std::unique_ptr<Expr> Parser::andExpression()
{
    std::unique_ptr<Expr> expr = equality();

    while ( match( { TokenType::AND } ) )
    {
        Token op = previous();
        std::unique_ptr<Expr> right = equality();
        expr = std::make_unique<Logical>( std::move( expr ), op,
                                          std::move( right ) );
    }

    return expr;
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

    return call();
}

std::unique_ptr<Expr> Parser::call()
{
    std::unique_ptr<Expr> expr = primary();

    while ( true )
    {
        if ( match( { TokenType::LEFT_PAREN } ) )
        {
            expr = finishCall( std::move( expr ) );
        }
        else if ( match( { TokenType::DOT } ) )
        {
            Token name = consume( TokenType::IDENTIFIER,
                                  "Expect property name after '.'" );
            expr = std::make_unique<Get>( std::move( expr ), name );
        }
        else
        {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::finishCall( std::unique_ptr<Expr> callee )
{
    std::vector<std::unique_ptr<Expr>> arguments{};

    if ( !check( TokenType::RIGHT_PAREN ) )
    {
        do
        {
            if ( arguments.size() >= 255 )
            {
                Error::error( peek(), "Can't have more than 255 arguments." );
            }
            arguments.push_back( std::move( expression() ) );
        } while ( match( { TokenType::COMMA } ) );
    }

    Token paren =
        consume( TokenType::RIGHT_PAREN, "Expect ')' after arguments." );

    return std::make_unique<Call>( std::move( callee ), paren,
                                   std::move( arguments ) );
}

std::unique_ptr<Expr> Parser::primary()
{
    if ( match( { TokenType::FALSE } ) )
        return std::make_unique<Literal>( Object{ false } );
    if ( match( { TokenType::TRUE } ) )
        return std::make_unique<Literal>( Object{ true } );
    if ( match( { TokenType::NIL } ) )
        return std::make_unique<Literal>( Object{ std::monostate{} } );

    if ( match( { TokenType::NUMBER, TokenType::STRING } ) )
        return std::make_unique<Literal>( Object{ previous().getLiteral() } );

    if ( match( { TokenType::SUPER } ) )
    {
        Token keyword = previous();
        consume( TokenType::DOT, "Expect '.' after 'super'." );
        Token method =
            consume( TokenType::IDENTIFIER, "Expect superclass method name." );
        return std::make_unique<Super>( keyword, method );
    }

    if ( match( { TokenType::THIS } ) )
        return std::make_unique<This>( previous() );

    if ( match( { TokenType::IDENTIFIER } ) )
        return std::make_unique<Variable>( previous() );

    if ( match( { TokenType::LEFT_PAREN } ) )
    {
        std::unique_ptr<Expr> expr = expression();
        consume( TokenType::RIGHT_PAREN, "Expect ')' after expression." );
        return std::make_unique<Grouping>( std::move( expr ) );
    }

    throw error( peek(), "Expect expression." );
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

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    std::vector<std::unique_ptr<Stmt>> statements{};
    while ( !isAtEnd() )
    {
        statements.push_back( std::move( declaration() ) );
    }

    return statements;
}