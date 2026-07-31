#pragma once

#include "Errors.h"
#include <string>

class JWT {

    static std::string SECRET;

public:

    static void setSecret(const std::string& secret);

    struct TokenInfo
    {
        bool valido;
        int adminId;
        std::string username;
        std::string role;
        JWTError error;
    };

    static std::string generarToken( int adminId, const std::string& username);
    static TokenInfo validarToken(const std::string& token);

};