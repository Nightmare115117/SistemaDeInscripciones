#pragma once

#include <sw/redis++/redis++.h>
#include <string>

class RedisC {

public:
    static void set(const std::string& key, const std::string& value);
    static std::string get(const std::string& key);
    static long long increment(const std::string& key);
    static long long publish(const std::string& channel, const std::string& message);
private:
    static std::string REDIS_URL;
    static sw::redis::Redis redis;
};