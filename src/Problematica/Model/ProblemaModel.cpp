#include "ProblemaModel.h"

using namespace std;

ProblemaModel::ProblemaModel() : ProblemaModel("", "", "") {}

ProblemaModel::ProblemaModel(const string& nombre, const string& descripcion)
    : Entity<int>(0), nombre(nombre), descripcion(descripcion), codigo("") {}

ProblemaModel::ProblemaModel(const string& nombre, const string& descripcion, const string& codigo)
    : Entity<int>(0), nombre(nombre), descripcion(descripcion), codigo(codigo) {}

const string& ProblemaModel::getNombre() const {
    return nombre;
}

const string& ProblemaModel::getDescripcion() const {
    return descripcion;
}

const string& ProblemaModel::getCodigo() const {
    return codigo;
}

void ProblemaModel::setNombre(const string& nombre) {
    this->nombre = nombre;
}

void ProblemaModel::setDescripcion(const string& descripcion) {
    this->descripcion = descripcion;
}

void ProblemaModel::setCodigo(const string& codigo) {
    this->codigo = codigo;
}
