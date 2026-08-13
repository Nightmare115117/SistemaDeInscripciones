#include "RedisClient.h"

#include <stdexcept>
#include <iostream>

using namespace std;

string Redis::REDIS_URL = [] {
    const char* envRedis = getenv("REDIS_URL");

    if (!envRedis || string(envRedis).empty()) {
        cerr << "REDIS_URL no configurada\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "REDIS_URL cargada correctamente\n";

    return std::string(envRedis);
}();