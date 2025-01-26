#pragma once
#include <algorithm>
#include <exception>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

#include "Error.h"
#include "Expression.h"
#include "Statement.h"
#include "Token.h"

class ParseError : public std::runtime_error
{
public:
    ParseError( const std::string& error );
};

class Parser
{
private:
    std::vector<Token> m_tokens;
    int m_current{ 0 };

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> expressionStatement();
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    bool match( std::initializer_list<TokenType::Type> types );
    Token& consume( TokenType::Type type, const std::string& message );
    bool check( TokenType::Type type );
    Token& advance();
    bool isAtEnd();
    Token& peek();
    Token& previous();
    ParseError error( Token token, const std::string& message );
    void synchronize();

public:
    Parser( const std::vector<Token>& tokens );

    std::vector<std::unique_ptr<Stmt>> parse();
};
