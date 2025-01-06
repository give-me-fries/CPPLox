#pragma once
#include <array>
#include <iostream>
#include <string>
#include <variant>

using Object = std::variant<std::monostate, std::string, double>;

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
        MAXTOKENTYPE
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
        "LOX_EOF" };
    static_assert( std::size( TYPES ) == Type::MAXTOKENTYPE );

    std::string getType( Type type );
} // namespace TokenType

std::string objectToString( const Object& obj );

class Token
{
public:
    Token( TokenType::Type type, std::string lexeme, Object literal, int line )
        : m_type{ type }, m_lexeme{ lexeme }, m_literal{ literal },
          m_line{ line }
    {
    }

    std::string toString() const;
    const std::string& getLexeme() const;

    friend std::ostream& operator<<( std::ostream& out, const Token& token );

private:
    std::string literalToString() const;

    const TokenType::Type m_type{ TokenType::MAXTOKENTYPE };
    const std::string m_lexeme{};
    const Object m_literal{ std::monostate{} };
    const int m_line{};
};