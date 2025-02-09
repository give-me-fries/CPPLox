#pragma once

#include <map>
#include <string>
#include <vector>

#include "Interpreter.h"
#include "LoxCallable.h"
#include "Object.h"

class LoxFunction;

class LoxClass : public LoxCallable
{
public:
    LoxClass(
        const std::string& name, std::shared_ptr<LoxClass> superclass,
        const std::map<std::string, std::shared_ptr<LoxFunction>>& methods )
        : m_name{ name }, superclass{ superclass }, m_methods{ methods }
    {
    }

    std::shared_ptr<LoxFunction> findMethod( const std::string& name ) const;
    int arity() const override;
    Object call( Interpreter& interpreter,
                 const std::vector<Object>& arguments ) override;
    std::string toString() const override;

private:
    std::string m_name;
    std::shared_ptr<LoxClass> superclass;
    std::map<std::string, std::shared_ptr<LoxFunction>> m_methods;
};
