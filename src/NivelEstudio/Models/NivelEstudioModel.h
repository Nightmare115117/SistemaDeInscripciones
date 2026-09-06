#pragma once
#include "Generics/Entity.h"
#include <string>

class NivelEstudioModel : public Entity<int> {
    std::string nivel;

public:
    NivelEstudioModel();
    explicit NivelEstudioModel(const std::string& nivel);

    const std::string& getNivel() const;
    void setNivel(const std::string& nivel);
};
