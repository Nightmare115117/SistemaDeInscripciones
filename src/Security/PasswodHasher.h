#pragma once
#include <string>

class PasswordHasher {

    int costo;

public:

    explicit PasswordHasher(int costo);

    std::string hash(const std::string& password) const;
    bool verificar(const std::string& passwordOriginal, const std::string& passwordHash) const;

};