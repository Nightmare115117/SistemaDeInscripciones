#include "RegistroModel.h"

using namespace std;

RegistroModel::RegistroModel() : RegistroModel(0){}

RegistroModel::RegistroModel(int idEquipo) : Entity(0) {
    this->idEquipo = idEquipo;
}

int RegistroModel::getIdEquipo() const{
    return idEquipo;
}

void RegistroModel::setIdEquipo(int idEquipo) {
    this->idEquipo = idEquipo;
}

const string& RegistroModel::getFechaHora() const {
 return fecha_hora;
}

void RegistroModel::setFecha(const string& fecha_hora) {
    this->fecha_hora = fecha_hora;
}