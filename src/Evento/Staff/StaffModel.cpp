#include "StaffModel.h"

const std::string& StaffModel::getId() const { return id; }
const std::string& StaffModel::getNombre() const { return nombre; }
const std::string& StaffModel::getRol() const { return rol; }
const std::string& StaffModel::getArea() const { return area; }
const std::string& StaffModel::getCorreo() const { return correo; }
const std::string& StaffModel::getTelefono() const { return telefono; }
void StaffModel::setId(const std::string& value) { id = value; }
void StaffModel::setNombre(const std::string& value) { nombre = value; }
void StaffModel::setRol(const std::string& value) { rol = value; }
void StaffModel::setArea(const std::string& value) { area = value; }
void StaffModel::setCorreo(const std::string& value) { correo = value; }
void StaffModel::setTelefono(const std::string& value) { telefono = value; }
