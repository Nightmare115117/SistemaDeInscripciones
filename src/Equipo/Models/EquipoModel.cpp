#include "EquipoModel.h"

using namespace std;

EquipoModel::EquipoModel() : EquipoModel("", -1, -1, -1) {}

EquipoModel::EquipoModel(const string& nombre)
    : Entity(0), nombre(nombre), idUniversidad(-1), idLider(-1), idProblematica(-1) {}

EquipoModel::EquipoModel(const string& nombre, int idUniversidad, int idLider, int idProblematica)
    : Entity(0), nombre(nombre), idUniversidad(idUniversidad), idLider(idLider), idProblematica(idProblematica) {}

const string& EquipoModel::getNombre() const {
    return nombre;
}

int EquipoModel::getIdUniversidad() const {
    return idUniversidad;
}

int EquipoModel::getIdLider() const {
    return idLider;
}

int EquipoModel::getIdProblematica() const {
    return idProblematica;
}

void EquipoModel::setNombre(const string& nombre) {
    this->nombre = nombre;
}

void EquipoModel::setIdUniversidad(int idUniversidad) {
    this->idUniversidad = idUniversidad;
}

void EquipoModel::setIdLider(int idLider) {
    this->idLider = idLider;
}

void EquipoModel::setIdProblematica(int idProblematica) {
    this->idProblematica = idProblematica;
}
