#pragma once

#include "crow.h"

class AuthMiddleware {

public:

    struct context {};

    void before_handle(crow::request& reg, crow::response& res, context& ctx);

    void after_handle(crow::request& reg, crow::response& res, context& ctx);

};