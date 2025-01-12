cmake_minimum_required(VERSION 3.15...3.31)

project(CPPLOX VERSION 1.0
        DESCRIPTION "C++ version of the Jlox interpreter"
        LANGUAGES CXX)

add_executable(
    cpplox
    src/main.cpp
    src/Driver.cpp
    src/Error.cpp
    src/Token.cpp
    src/Scanner.cpp
    src/ASTPrinter.cpp
    src/Expression.cpp
    src/Parser.cpp
    src/Interpreter.cpp)

set_target_properties(cpplox PROPERTIES CXX_STANDARD 17)
target_compile_options(cpplox PRIVATE -Wall -Wextra -Wconversion -Wpedantic -g)