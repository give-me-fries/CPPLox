#include <memory>
#include <vector>

#include "Error.h"
#include "Expression.h"
#include "Interpreter.h"
#include "Resolver.h"
#include "Statement.h"
#include "Token.h"

void Resolver::resolve( const std::vector<std::unique_ptr<Stmt>>& statements )
{
    for ( auto&& statement : statements )
    {
        resolve( statement.get() );
    }
}

void Resolver::visit( Block* stmt )
{
    beginScope();
    resolve( stmt->statements );
    endScope();
}

void Resolver::visit( ClassStmt* stmt )
{
    ClassType enclosingClass = m_currentClass;
    m_currentClass = ClassType::CLASS;

    declare( stmt->name );
    define( stmt->name );

    if ( stmt->superclass.get() &&
         stmt->name.getLexeme() == stmt->superclass->name.getLexeme() )
        Error::error( stmt->superclass->name,
                      "A class can't inherit from itself." );

    if ( stmt->superclass.get() )
    {
        m_currentClass = ClassType::SUBCLASS;
        resolve( stmt->superclass.get() );
    }

    if ( stmt->superclass.get() )
    {
        beginScope();
        m_scopes.back().emplace( "super", true );
    }

    beginScope();
    m_scopes.back().emplace( "this", true );

    for ( auto&& method : stmt->methods )
    {
        FunctionType declaration = FunctionType::METHOD;
        if ( method->name.getLexeme() == "init" )
            declaration = FunctionType::INITIALIZER;
        resolveFunction( method.get(), declaration );
    }

    endScope();

    if ( stmt->superclass.get() )
        endScope();

    m_currentClass = enclosingClass;
}

void Resolver::visit( Expression* stmt )
{
    resolve( stmt->expression.get() );
}

void Resolver::visit( Function* stmt )
{
    declare( stmt->name );
    define( stmt->name );

    resolveFunction( stmt, FunctionType::FUNCTION );
}

void Resolver::visit( If* stmt )
{
    resolve( stmt->condition.get() );
    resolve( stmt->thenBranch.get() );
    if ( stmt->elseBranch.get() )
        resolve( stmt->elseBranch.get() );
}

void Resolver::visit( Print* stmt )
{
    resolve( stmt->expression.get() );
}

void Resolver::visit( Return* stmt )
{
    if ( m_currentFunction == FunctionType::NONE )
    {
        Error::error( stmt->keyword, "Can't return from top-level code." );
    }

    if ( stmt->value.get() )
    {
        if ( m_currentFunction == FunctionType::INITIALIZER )
            Error::error( stmt->keyword,
                          "Can't return a value from an initializer." );
        resolve( stmt->value.get() );
    }
}

void Resolver::visit( Var* stmt )
{
    declare( stmt->name );
    if ( stmt->initializer.get() )
    {
        resolve( stmt->initializer.get() );
    }
    define( stmt->name );
}

void Resolver::visit( While* stmt )
{
    resolve( stmt->condition.get() );
    resolve( stmt->body.get() );
}

void Resolver::visit( Assign* expr )
{
    resolve( expr->value.get() );
    resolveLocal( expr, expr->name );
}

void Resolver::visit( Binary* expr )
{
    resolve( expr->left.get() );
    resolve( expr->right.get() );
}

void Resolver::visit( Call* expr )
{
    resolve( expr->callee.get() );

    for ( auto&& argument : expr->arguments )
    {
        resolve( argument.get() );
    }
}

void Resolver::visit( Get* expr )
{
    resolve( expr->object.get() );
}

void Resolver::visit( Grouping* expr )
{
    resolve( expr->expr.get() );
}

void Resolver::visit( Literal* )
{
    return;
}

void Resolver::visit( Logical* expr )
{
    resolve( expr->left.get() );
    resolve( expr->right.get() );
}

void Resolver::visit( Set* expr )
{
    resolve( expr->value.get() );
    resolve( expr->object.get() );
}

void Resolver::visit( Super* expr )
{
    if ( m_currentClass == ClassType::NONE )
        Error::error( expr->keyword, "Can't use 'super' outside of a class." );
    else if ( m_currentClass != ClassType::SUBCLASS )
        Error::error( expr->keyword,
                      "Can't use 'super' in a class with no superclass." );

    resolveLocal( expr, expr->keyword );
}

void Resolver::visit( This* expr )
{
    if ( m_currentClass == ClassType::NONE )
    {
        Error::error( expr->keyword, "Can't use 'this' outside of a class." );
        return;
    }

    resolveLocal( expr, expr->keyword );
}

void Resolver::visit( Unary* expr )
{
    resolve( expr->right.get() );
}

void Resolver::visit( Variable* expr )
{
    if ( !m_scopes.empty() &&
         m_scopes.back().find( expr->name.getLexeme() ) !=
             m_scopes.back().end() &&
         m_scopes.back().at( expr->name.getLexeme() ) == false )
    {
        Error::error( expr->name,
                      "Can't read local variable in its own initializer." );
    }

    resolveLocal( expr, expr->name );
}

void Resolver::resolve( Stmt* stmt )
{
    stmt->accept( this );
}

void Resolver::resolve( Expr* expr )
{
    expr->accept( this );
}

void Resolver::beginScope()
{
    m_scopes.emplace_back( std::map<std::string, bool>{} );
}

void Resolver::endScope()
{
    m_scopes.pop_back();
}

void Resolver::declare( const Token& name )
{
    if ( m_scopes.empty() )
        return;

    std::map<std::string, bool>& scope = m_scopes.back();
    if ( scope.find( name.getLexeme() ) != scope.end() )
    {
        Error::error( name,
                      "Already a variable with this name in this scope." );
    }

    scope.emplace( name.getLexeme(), false );
}

void Resolver::define( const Token& name )
{
    if ( m_scopes.empty() )
        return;

    m_scopes.back()[name.getLexeme()] = true;
}

void Resolver::resolveLocal( Expr* expr, const Token& name )
{
    for ( int i = static_cast<int>( m_scopes.size() - 1 ); i >= 0; --i )
    {
        if ( m_scopes[i].find( name.getLexeme() ) != m_scopes[i].end() )
        {
            m_interpreter.resolve(
                expr, static_cast<int>( m_scopes.size() - 1 - i ) );
            return;
        }
    }
}

void Resolver::resolveFunction( Function* function, FunctionType type )
{
    FunctionType enclosingFunction = m_currentFunction;
    m_currentFunction = type;

    beginScope();

    for ( const auto& param : function->params )
    {
        declare( param );
        define( param );
    }

    resolve( function->body );
    endScope();

    m_currentFunction = enclosingFunction;
}
