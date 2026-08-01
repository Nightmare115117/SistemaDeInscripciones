#pragma once

#include "crow.h"

class AuthMiddleware {

public:

    struct context {
        bool authenticated = false;
        int adminId = -1;
        std::string username;
        std::string role;
    };

    void before_handle(crow::request& reg, crow::response& res, context& ctx);

    void after_handle(crow::request& reg, crow::response& res, context& ctx);

};