#include "EquipoModel.h"

using namespace std;

EquipoModel::EquipoModel() : EquipoModel("", -1, -1, -1) {}

EquipoModel::EquipoModel(const string& nombre)
    : Entity(0), nombre(nombre), idUniversidad(-1), idLider(-1), idProblematica(-1), estado("pendiente") {}

EquipoModel::EquipoModel(const string& nombre, int idUniversidad, int idLider, int idProblematica)
    : Entity(0), nombre(nombre), idUniversidad(idUniversidad), idLider(idLider), idProblematica(idProblematica), estado("pendiente") {}

const string& EquipoModel::getNombre() const {
    return nombre;
}

int EquipoModel::getIdLider() const {
    return idLider;
}

int EquipoModel::getIdUniversidad() const {
    return idUniversidad;
}

int EquipoModel::getIdProblematica() const {
    return idProblematica;
}

const string& EquipoModel::getEstado() const { return estado; }
const string& EquipoModel::getRegistradoEn() const { return registradoEn; }
const string& EquipoModel::getRevisadoEn() const { return revisadoEn; }
const string& EquipoModel::getNotas() const { return notas; }

void EquipoModel::setNombre(const string& nombre) {
    this->nombre = nombre;
}

void EquipoModel::setIdLider(int idLider) {
    this->idLider = idLider;
}

void EquipoModel::setIdUniversidad(int idUniversidad) {
    this->idUniversidad = idUniversidad;
}

void EquipoModel::setIdProblematica(int idProblematica) {
    this->idProblematica = idProblematica;
}

void EquipoModel::setEstado(const string& value) { estado = value; }
void EquipoModel::setRegistradoEn(const string& value) { registradoEn = value; }
void EquipoModel::setRevisadoEn(const string& value) { revisadoEn = value; }
void EquipoModel::setNotas(const string& value) { notas = value; }
