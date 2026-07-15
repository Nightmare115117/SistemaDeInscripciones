#pragma once
#include <string>
#include "Generics/Entity.h"

class EquipoModel : public Entity {

    std::string nombre;
    int idUniversidad;
    int idLider;
    int idProblematica;

public:

    EquipoModel();
    explicit EquipoModel(const std::string& nombre);
    EquipoModel(const std::string& nombre, int idUniversidad, int idLider = -1, int idProblematica = -1);

    const std::string& getNombre() const;
    int getIdUniversidad() const;
    int getIdLider() const;
    int getIdProblematica() const;

    void setNombre(const std::string& nombre);
    void setIdUniversidad(int idUniversidad);
    void setIdLider(int idLider);
    void setIdProblematica(int idProblematica);
};
