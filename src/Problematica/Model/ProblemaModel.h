#pragma once
#include <string>
#include "Generics/Entity.h"

class ProblemaModel : public Entity {

    std::string nombre;
    std::string descripcion;

public:

    ProblemaModel();
    ProblemaModel(const std::string& nombre, const std::string& descripcion);

    const std::string& getNombre() const;
    const std::string& getDescripcion() const;

    void setNombre(const std::string& nombre);
    void setDescripcion(const std::string& descripcion);
};