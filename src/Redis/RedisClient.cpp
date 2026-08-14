#include "RedisClient.h"

#include <stdexcept>
#include <iostream>

using namespace std;
using namespace sw::redis;

string RedisC::REDIS_URL = [] {
    const char* envRedis = getenv("REDIS_URL");

    if (!envRedis || string(envRedis).empty()) {
        cerr << "REDIS_URL no configurada\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "REDIS_URL cargada correctamente\n";

    return std::string(envRedis);
}();

Redis RedisC::redis = []{
    return Redis(RedisC::REDIS_URL);
}();

void RedisC::set(const string& key, const string& value) {
    redis.set(key, value);
}

string RedisC::get(const string& key) {
    auto value = redis.get(key);

    if (value) {
        return *value;
    }

    return "";
}

long long RedisC::increment(const string& key) {
    return redis.incr(key);
}

long long RedisC::publish(const string& channel, const string& message) {
    return redis.publish(channel, message);
}