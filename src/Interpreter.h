#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Environment.h"
#include "Error.h"
#include "Expression.h"
#include "LoxFunction.h"
#include "Object.h"
#include "Statement.h"
#include "Token.h"
#include "Visitor.h"

class Interpreter : public IVisitor
{
public:
    friend class LoxFunction;

    Interpreter();

    void interpret( const std::vector<std::unique_ptr<Stmt>>& statements );
    void visit( Assign* expr ) override;
    void visit( Binary* expr ) override;
    void visit( Call* expr ) override;
    void visit( Get* expr ) override;
    void visit( Grouping* expr ) override;
    void visit( Literal* expr ) override;
    void visit( Logical* expr ) override;
    void visit( Set* expr ) override;
    void visit( This* expr ) override;
    void visit( Unary* expr ) override;
    void visit( Variable* expr ) override;

    void visit( Block* stmt ) override;
    void visit( ClassStmt* stmt ) override;
    void visit( Expression* stmt ) override;
    void visit( Function* stmt ) override;
    void visit( If* stmt ) override;
    void visit( Print* stmt ) override;
    void visit( Return* Stmt ) override;
    void visit( Var* stmt ) override;
    void visit( While* stmt ) override;

    void resolve( Expr* expr, int depth );

    friend Object LoxFunction::call( Interpreter& interpreter,
                                     const std::vector<Object>& arguments );

private:
    void evaluate( Expr* expr );
    void execute( Stmt* stmt );

    void executeBlock( const std::vector<std::unique_ptr<Stmt>>& statements,
                       std::shared_ptr<Environment> environment );
    void checkNumberOperand( const Token& op, const Object& operand );
    void checkNumberOperands( const Token& op, const Object& left,
                              const Object& right );
    bool isTruthy( const Object& object );
    bool isEqual( const Object& a, const Object& b );
    std::string stringify( const Object& object );

    Object lookUpVariable( const Token& name, Expr* expr );

    Object m_object{};
    std::shared_ptr<Environment> m_globals{ new Environment{} };
    std::shared_ptr<Environment> m_environment = m_globals;
    std::map<Expr*, int> m_locals{};
};
