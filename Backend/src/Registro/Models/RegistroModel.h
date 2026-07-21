#pragma once
#include <string>
#include "Generics/Entity.h"

class RegistroModel : public Entity {

    int idEquipo;
    std::string fecha_hora;

public:

    RegistroModel();
    explicit RegistroModel(int idEquipo);

    int getIdEquipo() const;
    void setIdEquipo(int idEquipo);
    const std::string& getFechaHora() const;
    void setFecha(const std::string& fecha_hora);
};