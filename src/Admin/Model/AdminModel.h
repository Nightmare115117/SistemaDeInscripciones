#pragma once

#include <string>
#include "Generics/Entity.h"

class AdminModel : public Entity {

    std::string nombre;
    std::string correo;
    std::string contraseña;

public:

    AdminModel();
    AdminModel(const std::string& nombre);
    explicit AdminModel(
        const std::string& nombre, const std::string& correo, 
        const std::string& contraseña
    );

    const std::string& getNombre() const;
    const std::string& getCorreo() const;
    const std::string& getContrasena() const;

    void setNombre(const std::string& nombre);
    void setCorreo(const std::string& correo);
    void setContrasena(const std::string& Contrasena);
};