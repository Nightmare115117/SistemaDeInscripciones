#include "RateLimitMiddleware.h"

#include <algorithm>
#include <cstdlib>
#include <limits>

RateLimitMiddleware::RateLimitMiddleware()
    : maxRequests_(readPositiveEnv("RATE_LIMIT_MAX_REQUESTS", 100)),
      windowDuration_(readPositiveEnv("RATE_LIMIT_WINDOW_SECONDS", 60)) {}

unsigned int RateLimitMiddleware::readPositiveEnv(const char* name, unsigned int fallback) {
    const char* value = std::getenv(name);
    if (value == nullptr || *value == '\0') {
        return fallback;
    }

    char* end = nullptr;
    const unsigned long parsed = std::strtoul(value, &end, 10);
    if (end == value || *end != '\0' || parsed == 0 || parsed > std::numeric_limits<unsigned int>::max()) {
        return fallback;
    }

    return static_cast<unsigned int>(parsed);
}

void RateLimitMiddleware::before_handle(crow::request& req, crow::response& res, context&) {
    const auto now = std::chrono::steady_clock::now();
    const std::string clientIp = req.remote_ip_address;

    std::lock_guard<std::mutex> lock(mutex_);

    for (auto it = windows_.begin(); it != windows_.end();) {
        if (now - it->second.started >= windowDuration_) {
            it = windows_.erase(it);
        } else {
            ++it;
        }
    }

    auto& window = windows_[clientIp];
    if (window.requests == 0 || now - window.started >= windowDuration_) {
        window.started = now;
        window.requests = 0;
    }

    if (window.requests >= maxRequests_) {
        const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - window.started);
        const auto retryAfter = std::max<long long>(1, windowDuration_.count() - elapsed.count());

        res.code = 429;
        res.set_header("Content-Type", "application/json");
        res.set_header("Retry-After", std::to_string(retryAfter));
        res.write(R"({"detail":"Límite de solicitudes excedido"})");
        res.end();
        return;
    }

    ++window.requests;
}

void RateLimitMiddleware::after_handle(crow::request&, crow::response&, context&) {}