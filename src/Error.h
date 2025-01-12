#pragma once
#include <string>
#include <string_view>

#include "Token.h"

namespace Error
{
    class RuntimeError : public std::exception
    {
    private:
        Token m_token;
        std::string m_error{};

    public:
        RuntimeError( Token token, std::string_view error );
        const char* what() const noexcept override;
        const Token& getToken() const;
    };

    inline bool hadError = false;
    inline bool hadRuntimeError = false;
    void error( int line, const std::string& message );
    void error( Token token, const std::string& message );
    void runtimeError( const RuntimeError& error );
    void report( int line, const std::string& where,
                 const std::string& message );
} // namespace Error