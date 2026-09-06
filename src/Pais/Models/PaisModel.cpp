#include "PaisModel.h"

PaisModel::PaisModel() : Entity<int>(0) {}

PaisModel::PaisModel(const std::string& nombre, const std::string& codigoIso2,
    const std::string& codigoIso3)
    : Entity<int>(0), nombre(nombre), codigoIso2(codigoIso2), codigoIso3(codigoIso3) {}

const std::string& PaisModel::getNombre() const {
    return nombre;
}

const std::string& PaisModel::getCodigoIso2() const {
    return codigoIso2;
}

const std::string& PaisModel::getCodigoIso3() const {
    return codigoIso3;
}

void PaisModel::setNombre(const std::string& value) {
    nombre = value;
}

void PaisModel::setCodigoIso2(const std::string& value) {
    codigoIso2 = value;
}

void PaisModel::setCodigoIso3(const std::string& value) {
    codigoIso3 = value;
}
