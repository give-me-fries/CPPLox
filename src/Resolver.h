#pragma once
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "Expression.h"
#include "Visitor.h"

class Interpreter;
class Token;
struct Expr;
struct Stmt;

class Resolver : public IVisitor
{
public:
    Resolver( Interpreter& interpreter ) : m_interpreter{ interpreter }
    {
    }

    void resolve( const std::vector<std::unique_ptr<Stmt>>& statements );
    void visit( Block* stmt ) override;
    void visit( ClassStmt* stmt ) override;
    void visit( Expression* stmt ) override;
    void visit( Function* stmt ) override;
    void visit( If* stmt ) override;
    void visit( Print* stmt ) override;
    void visit( Return* stmt ) override;
    void visit( Var* stmt ) override;
    void visit( While* stmt ) override;

    void visit( Assign* expr ) override;
    void visit( Binary* expr ) override;
    void visit( Call* expr ) override;
    void visit( Get* expr ) override;
    void visit( Grouping* expr ) override;
    void visit( Literal* ) override;
    void visit( Logical* expr ) override;
    void visit( Set* expr ) override;
    void visit( Super* expr ) override;
    void visit( This* expr ) override;
    void visit( Unary* expr ) override;
    void visit( Variable* expr ) override;

private:
    enum class FunctionType
    {
        NONE,
        FUNCTION,
        INITIALIZER,
        METHOD
    };

    enum class ClassType
    {
        NONE,
        CLASS,
        SUBCLASS
    };

    void resolve( Stmt* stmt );
    void resolve( Expr* expr );
    void beginScope();
    void endScope();
    void declare( const Token& name );
    void define( const Token& name );
    void resolveLocal( Expr* expr, const Token& name );
    void resolveFunction( Function* function, FunctionType type );

    Interpreter& m_interpreter;
    std::vector<std::map<std::string, bool>> m_scopes{};
    FunctionType m_currentFunction{ FunctionType::NONE };
    ClassType m_currentClass{ ClassType::NONE };
};
