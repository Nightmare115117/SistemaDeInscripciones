#pragma once
#include <string>
#include "Generics/Entity.h"

class ProblemaModel : public Entity<int> {

    std::string nombre;
    std::string descripcion;
    std::string codigo;

public:

    ProblemaModel();
    ProblemaModel(const std::string& nombre, const std::string& descripcion);
    ProblemaModel(const std::string& nombre, const std::string& descripcion, const std::string& codigo);

    const std::string& getNombre() const;
    const std::string& getDescripcion() const;
    const std::string& getCodigo() const;

    void setNombre(const std::string& nombre);
    void setDescripcion(const std::string& descripcion);
    void setCodigo(const std::string& codigo);
};