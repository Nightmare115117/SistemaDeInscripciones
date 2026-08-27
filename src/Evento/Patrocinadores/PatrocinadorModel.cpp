#include "PatrocinadorModel.h"

const std::string& PatrocinadorModel::getId() const { return Entity<std::string>::getId(); }
const std::string& PatrocinadorModel::getNombre() const { return nombre; }
const std::string& PatrocinadorModel::getLogo() const { return logo; }
const std::string& PatrocinadorModel::getUrl() const { return url; }
const std::string& PatrocinadorModel::getNivel() const { return nivel; }
void PatrocinadorModel::setId(const std::string& value) { Entity<std::string>::setId(value); }
void PatrocinadorModel::setNombre(const std::string& value) { nombre = value; }
void PatrocinadorModel::setLogo(const std::string& value) { logo = value; }
void PatrocinadorModel::setUrl(const std::string& value) { url = value; }
void PatrocinadorModel::setNivel(const std::string& value) { nivel = value; }
