#include "NivelEstudioModel.h"

NivelEstudioModel::NivelEstudioModel() : Entity<int>(0) {}

NivelEstudioModel::NivelEstudioModel(const std::string& nivel)
    : Entity<int>(0), nivel(nivel) {}

const std::string& NivelEstudioModel::getNivel() const {
    return nivel;
}

void NivelEstudioModel::setNivel(const std::string& value) {
    nivel = value;
}
