#include <iostream>
#include <memory>
#include <string>
#include <variant>

#include "Environment.h"
#include "Error.h"
#include "Expression.h"
#include "Interpreter.h"
#include "LoxCallable.h"
#include "LoxClock.h"
#include "LoxFunction.h"
#include "Object.h"
#include "ReturnValue.h"
#include "Statement.h"
#include "Token.h"

Interpreter::Interpreter()
{
    m_globals->define( "clock", Object{ std::make_shared<LoxClock>(
                                    std::move( LoxClock{} ) ) } );
}

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
    m_environment->assign( expr->name, m_object );
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

void Interpreter::visit( Call* expr )
{
    evaluate( expr->callee.get() );
    Object callee = m_object;

    std::vector<Object> arguments{};
    for ( auto&& argument : expr->arguments )
    {
        evaluate( argument.get() );
        arguments.push_back( m_object );
    }

    std::shared_ptr<LoxCallable> function;

    if ( !std::holds_alternative<std::shared_ptr<LoxCallable>>( callee ) )
    {
        throw Error::RuntimeError{ expr->paren,
                                   "Can only call functions and classes." };
    }

    function = std::get<std::shared_ptr<LoxCallable>>( callee );

    if ( static_cast<int>( arguments.size() ) != function->arity() )
    {
        throw Error::RuntimeError{
            expr->paren, "Expected " + std::to_string( function->arity() ) +
                             " arguments but got " +
                             std::to_string( arguments.size() ) + "." };
    }

    m_object = function->call( *this, arguments );
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

void Interpreter::visit( Logical* expr )
{
    evaluate( expr->left.get() );
    Object left = m_object;

    if ( expr->op.getType() == TokenType::OR )
    {
        if ( isTruthy( left ) )
        {
            m_object = left;
            return;
        }
    }
    else
    {
        if ( !isTruthy( left ) )
        {
            m_object = left;
            return;
        }
    }

    evaluate( expr->right.get() );
}

void Interpreter::visit( Variable* expr )
{
    m_object = m_environment->get( expr->name );
}

void Interpreter::visit( Block* stmt )
{
    std::shared_ptr<Environment> env{ new Environment{ m_environment } };
    executeBlock( stmt->statements, env );
}

void Interpreter::visit( Expression* stmt )
{
    evaluate( stmt->expression.get() );
}

void Interpreter::visit( Function* stmt )
{
    std::shared_ptr<LoxFunction> function{
        new LoxFunction{ stmt, m_environment } };
    m_environment->define( stmt->name.getLexeme(), Object{ function } );
}

void Interpreter::visit( If* stmt )
{
    evaluate( stmt->condition.get() );
    if ( isTruthy( m_object ) )
    {
        execute( stmt->thenBranch.get() );
    }
    else if ( stmt->elseBranch.get() )
    {
        execute( stmt->elseBranch.get() );
    }
}

void Interpreter::visit( Print* stmt )
{
    evaluate( stmt->expression.get() );
    std::cout << stringify( m_object ) << '\n';
}

void Interpreter::visit( Return* stmt )
{
    Object value{ std::monostate{} };
    if ( stmt->value != nullptr )
    {
        evaluate( stmt->value.get() );
        value = m_object;
    }

    throw ReturnValue{ value };
}

void Interpreter::visit( Var* stmt )
{
    Object value = std::monostate{};
    if ( stmt->initializer.get() )
    {
        evaluate( stmt->initializer.get() );
        value = m_object;
    }

    m_environment->define( stmt->name.getLexeme(), value );
}

void Interpreter::visit( While* stmt )
{
    evaluate( stmt->condition.get() );
    while ( isTruthy( m_object ) )
    {
        execute( stmt->body.get() );
        evaluate( stmt->condition.get() );
    }
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
    std::shared_ptr<Environment> previous = m_environment;

    try
    {
        m_environment = environment;

        for ( auto&& statement : statements )
        {
            execute( statement.get() );
        }
    }
    catch ( ... )
    {
        m_environment = previous;
        throw;
    }

    m_environment = previous;
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
    // Both null
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
    if ( a.index() == 3 && b.index() == 3 )
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
