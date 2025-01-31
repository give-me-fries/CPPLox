#pragma once

struct Assign;
struct Binary;
struct Call;
struct Grouping;
struct Literal;
struct Logical;
struct Unary;
struct Variable;

struct Block;
struct Expression;
struct Function;
struct If;
struct Print;
struct Return;
struct Var;
struct While;

struct IVisitor
{
    virtual void visit( Assign* ) = 0;
    virtual void visit( Binary* ) = 0;
    virtual void visit( Call* ) = 0;
    virtual void visit( Grouping* ) = 0;
    virtual void visit( Literal* ) = 0;
    virtual void visit( Logical* ) = 0;
    virtual void visit( Unary* ) = 0;
    virtual void visit( Variable* ) = 0;

    virtual void visit( Block* ) = 0;
    virtual void visit( Expression* ) = 0;
    virtual void visit( Function* ) = 0;
    virtual void visit( If* ) = 0;
    virtual void visit( Print* ) = 0;
    virtual void visit( Return* ) = 0;
    virtual void visit( Var* ) = 0;
    virtual void visit( While* ) = 0;

    virtual ~IVisitor()
    {
    }
};