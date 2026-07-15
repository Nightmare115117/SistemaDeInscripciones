#include "ContactoEmergenciaModel.h"

using namespace std;

ContactoEmergenciaModel::ContactoEmergenciaModel()
    : Entity(0), nombre(""), telefono(""), pariente("") {}

ContactoEmergenciaModel::ContactoEmergenciaModel(const string& nombre, const string& telefono, const string& pariente)
    : Entity(0), nombre(nombre), telefono(telefono), pariente(pariente) {}

const string& ContactoEmergenciaModel::getNombre() const {
    return nombre;
}

const string& ContactoEmergenciaModel::getTelefono() const {
    return telefono;
}

const string& ContactoEmergenciaModel::getPariente() const {
    return pariente;
}

void ContactoEmergenciaModel::setNombre(const string& nombre) {
    this->nombre = nombre;
}

void ContactoEmergenciaModel::setTelefono(const string& telefono) {
    this->telefono = telefono;
}

void ContactoEmergenciaModel::setPariente(const string& pariente) {
    this->pariente = pariente;
}
