#include <memory>
#include <string>
#include <vector>

#include "Interpreter.h"
#include "LoxClass.h"
#include "LoxFunction.h"
#include "LoxInstance.h"
#include "Object.h"

std::shared_ptr<LoxFunction> LoxClass::findMethod(
    const std::string& name ) const
{
    if ( m_methods.find( name ) != m_methods.end() )
        return m_methods.at( name );

    if ( superclass )
        return superclass->findMethod( name );

    return nullptr;
}

int LoxClass::arity() const
{
    std::shared_ptr<LoxFunction> initializer = findMethod( "init" );
    if ( !initializer.get() )
        return 0;
    return initializer->arity();
}

Object LoxClass::call( Interpreter& interpreter,
                       const std::vector<Object>& arguments )
{
    std::shared_ptr<LoxInstance> instance =
        std::make_shared<LoxInstance>( LoxInstance{ this } );
    std::shared_ptr<LoxFunction> initializer = findMethod( "init" );
    if ( initializer.get() )
        initializer->bind( instance )->call( interpreter, arguments );
    return instance;
}

std::string LoxClass::toString() const
{
    return m_name;
}