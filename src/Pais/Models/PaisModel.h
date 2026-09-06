#pragma once
#include "Generics/Entity.h"
#include <string>

class PaisModel : public Entity<int> {
    std::string nombre;
    std::string codigoIso2;
    std::string codigoIso3;

public:
    PaisModel();
    PaisModel(const std::string& nombre, const std::string& codigoIso2,
        const std::string& codigoIso3);

    const std::string& getNombre() const;
    const std::string& getCodigoIso2() const;
    const std::string& getCodigoIso3() const;
    void setNombre(const std::string& nombre);
    void setCodigoIso2(const std::string& codigoIso2);
    void setCodigoIso3(const std::string& codigoIso3);
};
