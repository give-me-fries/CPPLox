#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "Environment.h"
#include "LoxCallable.h"
#include "Object.h"
#include "Statement.h"

class Interpreter;

class LoxFunction : public LoxCallable
{
public:
    LoxFunction( Function* declaration, std::shared_ptr<Environment> closure,
                 bool isInitializer )
        : declaration{ declaration }, closure{ closure },
          m_isInitializer{ isInitializer }
    {
    }

    std::shared_ptr<LoxFunction> bind( std::shared_ptr<LoxInstance> instance );
    int arity() const override;
    Object call( Interpreter& interpreter,
                 const std::vector<Object>& arguments ) override;
    std::string toString() const override;

private:
    // Function is owned by a unique pointer, so this is fine. Maybe change
    // AST to be made of shared_ptr?
    Function* declaration;
    std::shared_ptr<Environment> closure;
    bool m_isInitializer;
};
