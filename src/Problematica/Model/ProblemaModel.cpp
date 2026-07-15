#include "ProblemaModel.h"

using namespace std;

ProblemaModel::ProblemaModel() : ProblemaModel("", "") {}

ProblemaModel::ProblemaModel(const string& nombre, const string& descripcion)
    : Entity(0), nombre(nombre), descripcion(descripcion) {}

const string& ProblemaModel::getNombre() const {
    return nombre;
}

const string& ProblemaModel::getDescripcion() const {
    return descripcion;
}

void ProblemaModel::setNombre(const string& nombre) {
    this->nombre = nombre;
}

void ProblemaModel::setDescripcion(const string& descripcion) {
    this->descripcion = descripcion;
}
