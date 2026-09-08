#pragma once

#include "crow.h"
#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>

class RateLimitMiddleware {
public:
    struct context {};

    RateLimitMiddleware();

    void before_handle(crow::request& req, crow::response& res, context& ctx);
    void after_handle(crow::request& req, crow::response& res, context& ctx);

private:
    struct Window {
        std::chrono::steady_clock::time_point started;
        unsigned int requests = 0;
    };

    static unsigned int readPositiveEnv(const char* name, unsigned int fallback);

    const unsigned int maxRequests_;
    const std::chrono::seconds windowDuration_;
    std::mutex mutex_;
    std::unordered_map<std::string, Window> windows_;
};