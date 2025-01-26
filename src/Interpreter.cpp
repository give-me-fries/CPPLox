#include <iostream>
#include <memory>
#include <string>
#include <variant>

#include "Environment.h"
#include "Error.h"
#include "Expression.h"
#include "Interpreter.h"
#include "Statement.h"
#include "Token.h"

void Interpreter::interpret(
    const std::vector<std::unique_ptr<Stmt>>& statements )
{
    try
    {
        for ( auto&& statement : statements )
        {
            execute( statement.get() );
        }
    }
    catch ( const Error::RuntimeError& error )
    {
        runtimeError( error );
    }
}

void Interpreter::visit( Assign* expr )
{
    evaluate( expr->value.get() );
    environment.assign( expr->name, m_object );
}

void Interpreter::visit( Binary* expr )
{
    evaluate( expr->left.get() );
    Object left = m_object;

    evaluate( expr->right.get() );
    Object right = m_object;

    switch ( expr->op.getType() )
    {
    case TokenType::GREATER:
        checkNumberOperands( expr->op, left, right );
        m_object = std::get<double>( left ) > std::get<double>( right );
        return;
    case TokenType::GREATER_EQUAL:
        checkNumberOperands( expr->op, left, right );
        m_object = std::get<double>( left ) >= std::get<double>( right );
        return;
    case TokenType::LESS:
        checkNumberOperands( expr->op, left, right );
        m_object = std::get<double>( left ) < std::get<double>( right );
        return;
    case TokenType::LESS_EQUAL:
        checkNumberOperands( expr->op, left, right );
        m_object = std::get<double>( left ) <= std::get<double>( right );
        return;
    case TokenType::BANG_EQUAL:
        m_object = !isEqual( left, right );
        return;
    case TokenType::EQUAL_EQUAL:
        m_object = isEqual( left, right );
        return;
    case TokenType::MINUS:
        checkNumberOperands( expr->op, left, right );
        m_object = std::get<double>( left ) - std::get<double>( right );
        return;
    case TokenType::PLUS:
        // Doubles
        if ( left.index() == 2 && right.index() == 2 )
        {
            m_object = std::get<double>( left ) + std::get<double>( right );
            return;
        }
        // Strings
        if ( left.index() == 1 && right.index() == 1 )
        {
            m_object =
                std::get<std::string>( left ) + std::get<std::string>( right );
            return;
        }

        throw Error::RuntimeError{
            expr->op, "Operands must be two numbers or two strings." };
    case TokenType::SLASH:
        checkNumberOperands( expr->op, left, right );
        m_object = std::get<double>( left ) / std::get<double>( right );
        return;
    case TokenType::STAR:
        checkNumberOperands( expr->op, left, right );
        m_object = std::get<double>( left ) * std::get<double>( right );
        return;
    default:
        m_object = std::monostate{};
        return;
    }
}

void Interpreter::visit( Grouping* expr )
{
    evaluate( expr->expr.get() );
}

void Interpreter::visit( Literal* expr )
{
    m_object = expr->value;
}

void Interpreter::visit( Unary* expr )
{
    evaluate( expr->right.get() );

    Object right = m_object;

    switch ( expr->op.getType() )
    {
    case TokenType::BANG:
        m_object = !isTruthy( right );
        return;
    case TokenType::MINUS:
        checkNumberOperand( expr->op, right );
        m_object = -std::get<double>( right );
        return;
    default:
        m_object = std::monostate{};
        return;
    }
}

void Interpreter::visit( Variable* expr )
{
    m_object = environment.get( expr->name );
}

void Interpreter::visit( Block* stmt )
{
    executeBlock( stmt->statements,
                  std::make_shared<Environment>( environment ) );
}

void Interpreter::visit( Expression* stmt )
{
    evaluate( stmt->expression.get() );
}

void Interpreter::visit( Print* stmt )
{
    evaluate( stmt->expression.get() );
    std::cout << stringify( m_object ) << '\n';
}

void Interpreter::visit( Var* stmt )
{
    Object value = std::monostate{};
    if ( stmt->initializer.get() )
    {
        evaluate( stmt->initializer.get() );
        value = m_object;
    }

    environment.define( stmt->name.getLexeme(), value );
}

void Interpreter::evaluate( Expr* expr )
{
    expr->accept( this );
}

void Interpreter::execute( Stmt* stmt )
{
    stmt->accept( this );
}

void Interpreter::executeBlock(
    const std::vector<std::unique_ptr<Stmt>>& statements,
    std::shared_ptr<Environment> environment )
{
    auto previous = this->environment;

    try
    {
        this->environment = environment;

        for ( auto&& statement : statements )
        {
            execute( statement.get() );
        }
    }
    catch ( ... )
    {
        this->environment = previous;
        throw;
    }

    this->environment = previous;
}

void Interpreter::checkNumberOperand( const Token& op, const Object& operand )
{
    if ( operand.index() == 2 )
        return;

    throw Error::RuntimeError{ op, "Operand must be a number." };
}

void Interpreter::checkNumberOperands( const Token& op, const Object& left,
                                       const Object& right )
{
    if ( left.index() == 2 && right.index() == 2 )
        return;

    throw Error::RuntimeError{ op, "Operands must be numbers." };
}

bool Interpreter::isTruthy( const Object& object )
{
    if ( object.index() == 0 )
        return false;
    if ( object.index() == 3 )
        return std::get<bool>( object );

    return true;
}

bool Interpreter::isEqual( const Object& a, const Object& b )
{
    // Both are null
    if ( a.index() == 0 && b.index() == 0 )
        return true;

    // A or B is null
    if ( a.index() == 0 || b.index() == 0 )
        return false;

    // Strings
    if ( a.index() == 1 && b.index() == 1 )
    {
        return std::get<std::string>( a ) == std::get<std::string>( b );
    }

    // Doubles
    if ( a.index() == 2 && b.index() == 2 )
    {
        return std::get<double>( a ) == std::get<double>( b );
    }

    // Booleans
    if ( a.index() == 3 && a.index() == 3 )
    {
        return std::get<bool>( a ) == std::get<bool>( b );
    }

    // None
    return false;
}

std::string Interpreter::stringify( const Object& object )
{
    if ( object.index() == 0 )
        return "nil";
    else
        return objectToString( object );
}