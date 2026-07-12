#pragma once
#include "Generics/Entity.h"
#include <string>

class UniversityModel : public Entity {

    std::string nombre;

public:

    UniversityModel();
    explicit UniversityModel(const std::string& nombre);

    const std::string& getNombre() const;

    void setNombre(const std::string& nombre);
};