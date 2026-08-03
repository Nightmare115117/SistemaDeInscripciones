#pragma once

#include "string"

class AES {

    static std::string KEY;

public:

    static std::string encrypt(const std::string& dato);
    static std::string decrypt(const std::string& dato);

};