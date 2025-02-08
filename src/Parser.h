#pragma once
#include <algorithm>
#include <exception>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>
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
    std::unique_ptr<Expr> orExpression();
    std::unique_ptr<Expr> andExpression();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> classDeclaration();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> forStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> returnStatement();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Function> function( const std::string& kind );
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> finishCall( std::unique_ptr<Expr> expr );
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
