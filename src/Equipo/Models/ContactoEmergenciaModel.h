#pragma once
#include <string>
#include "Generics/Entity.h"

class ContactoEmergenciaModel : public Entity {

    std::string nombre;
    std::string telefono;
    std::string pariente;

public:

    ContactoEmergenciaModel();
    ContactoEmergenciaModel(const std::string& nombre, const std::string& telefono, const std::string& pariente);

    const std::string& getNombre() const;
    const std::string& getTelefono() const;
    const std::string& getPariente() const;

    void setNombre(const std::string& nombre);
    void setTelefono(const std::string& telefono);
    void setPariente(const std::string& pariente);
};