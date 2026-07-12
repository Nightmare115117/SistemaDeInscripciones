#include "AlumnoModel.h"

using namespace std;

AlumnoModel::AlumnoModel() : AlumnoModel("", 0) {}

AlumnoModel::AlumnoModel(const string& nombre, int idEquipo) : Entity(0) {
    this->nombre = nombre;
    this->idEquipo = idEquipo;
}

const string& AlumnoModel::getNombre() const {
    return nombre;
}

int AlumnoModel::getIdEquipo() const {
    return idEquipo;
}

void AlumnoModel::setIdEquipo(int idEquipo) {
    this->idEquipo = idEquipo;
} 

void AlumnoModel::setNombre(const string& nombre) {
    this->nombre = nombre;
}