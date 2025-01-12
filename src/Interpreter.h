#pragma once
#include <memory>
#include <string>

#include "Error.h"
#include "Expression.h"
#include "Token.h"

class Interpreter : public IVisitor
{
public:
    void interpret( Expr* expr );
    void visit( Literal* expr ) override;
    void visit( Unary* expr ) override;
    void visit( Grouping* expr ) override;
    void visit( Binary* expr ) override;

private:
    void evaluate( Expr* expr );
    void checkNumberOperand( const Token& op, const Object& operand );
    void checkNumberOperands( const Token& op, const Object& left,
                              const Object& right );
    bool isTruthy( const Object& object );
    bool isEqual( const Object& a, const Object& b );
    std::string stringify( const Object& object );

    Object m_object{};
};
