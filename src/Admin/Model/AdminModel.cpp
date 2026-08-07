#include "AdminModel.h"

using namespace std;

AdminModel::AdminModel() : AdminModel("") {}

AdminModel::AdminModel(const string& nombre):
    AdminModel(nombre, "", "") {}

AdminModel::AdminModel(const string& nombre, const string& correo, const string& contrasena) : Entity(-1) {
    this->nombre = nombre;
    this->correo = correo;
    this->contraseña = contrasena;
}

const string& AdminModel::getNombre() const {
    return nombre;
}

const string& AdminModel::getCorreo() const {
    return correo;
}

const string& AdminModel::getCorreoAES() const {
    return correoAES;
}

const string& AdminModel::getCorreoHMAC() const {
    return correoHMAC;
}

const string& AdminModel::getContrasena() const {
    return contraseña;
}


void AdminModel::setNombre(const string& nombre) {
    this->nombre = nombre;
}

void AdminModel::setCorreo(const string& correo) {
    this->correo = correo;
}

void AdminModel::setCorreoAES(const string& correoAES) {
    this->correoAES = correoAES;
}

void AdminModel::setCorreoHMAC(const string& correoHMAC) {
    this->correoHMAC = correoHMAC;
}

void AdminModel::setContrasena(const string& contrasena) {
    this->contraseña = contrasena;
}