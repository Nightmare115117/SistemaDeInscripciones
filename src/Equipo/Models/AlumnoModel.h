#pragma once
#include <string>
#include "Generics/Entity.h"

class AlumnoModel : public Entity {

    std::string nombre;
    int idEquipo;

public:

    AlumnoModel();
    explicit AlumnoModel(const std::string& nombre, int idEquipo);

    const std::string& getNombre() const;
    int getIdEquipo() const;

    void setNombre(const std::string& nombre);
    void setIdEquipo(int idEquipo);
};