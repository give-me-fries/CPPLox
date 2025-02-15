#pragma once
#include <array>
#include <iostream>
#include <string>
#include <variant>

#include "LoxCallable.h"
#include "Object.h"

namespace TokenType
{
    enum Type
    {
        // Single-character tokens.
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACE,
        RIGHT_BRACE,
        COMMA,
        DOT,
        MINUS,
        PLUS,
        SEMICOLON,
        SLASH,
        STAR,

        // One or two character tokens.
        BANG,
        BANG_EQUAL,
        EQUAL,
        EQUAL_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,

        // Literals.
        IDENTIFIER,
        STRING,
        NUMBER,

        // Keywords.
        AND,
        CLASS,
        ELSE,
        FALSE,
        FUN,
        FOR,
        IF,
        NIL,
        OR,
        PRINT,
        RETURN,
        SUPER,
        THIS,
        TRUE,
        VAR,
        WHILE,

        // End of file.
        LOX_EOF,

        // Max enumerations.
        MAX_TOKENTYPE
    };

    using namespace std::string_literals;
    constexpr std::array TYPES{
        // Single-character tokens.
        "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "COMMA",
        "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",

        // One or two character tokens.
        "BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL", "GREATER",
        "GREATER_EQUAL", "LESS", "LESS_EQUAL",

        // Literals.
        "IDENTIFIER", "STRING", "NUMBER",

        // Keywords.
        "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
        "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",

        // End of file.
        "EOF" };
    static_assert( std::size( TYPES ) == Type::MAX_TOKENTYPE );

    std::string getType( Type type );
} // namespace TokenType

class Token
{
public:
    Token( TokenType::Type type, std::string lexeme, Object literal, int line )
        : m_type{ type }, m_lexeme{ lexeme }, m_literal{ literal },
          m_line{ line }
    {
    }

    std::string toString() const;
    std::string getLexeme() const;
    TokenType::Type getType() const;
    int getLine() const;
    friend std::ostream& operator<<( std::ostream& out, const Token& token );
    const Object& getLiteral() const;

private:
    std::string literalToString() const;
    const TokenType::Type m_type{ TokenType::MAX_TOKENTYPE };
    const std::string m_lexeme{};
    const Object m_literal{ std::monostate{} };
    const int m_line{};
};