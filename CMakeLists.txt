cmake_minimum_required(VERSION 3.15...3.31)

project(CPPLOX VERSION 1.0
        DESCRIPTION "C++ version of the Jlox interpreter"
        LANGUAGES CXX)

add_executable(
    cpplox
    src/ASTPrinter.cpp
    src/Driver.cpp
    src/Environment.cpp
    src/Error.cpp
    src/Expression.cpp
    src/Interpreter.cpp
    src/LoxClock.cpp
    src/LoxFunction.cpp
    src/main.cpp
    src/Parser.cpp
    src/Scanner.cpp
    src/Statement.cpp
    src/Token.cpp
)

set_target_properties(cpplox PROPERTIES CXX_STANDARD 17)
target_compile_options(cpplox PRIVATE -Wall -Wextra -Wconversion -Wpedantic -g)