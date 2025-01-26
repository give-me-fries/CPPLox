#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Environment.h"
#include "Error.h"
#include "Expression.h"
#include "Statement.h"
#include "Token.h"
#include "Visitor.h"

class Interpreter : public IVisitor
{
public:
    void interpret( const std::vector<std::unique_ptr<Stmt>>& statements );
    void visit( Literal* expr ) override;
    void visit( Unary* expr ) override;
    void visit( Grouping* expr ) override;
    void visit( Binary* expr ) override;
    void visit( Variable* expr ) override;
    void visit( Assign* expr ) override;

    void visit( Block* stmt ) override;
    void visit( Expression* stmt ) override;
    void visit( Print* stmt ) override;
    void visit( Var* stmt ) override;

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

    Object m_object{};
    Environment environment{};
};
