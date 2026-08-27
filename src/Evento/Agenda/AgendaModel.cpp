#include "AgendaModel.h"

const std::string& AgendaModel::getId() const { return Entity<std::string>::getId(); }
const std::string& AgendaModel::getDia() const { return dia; }
const std::string& AgendaModel::getInicio() const { return inicio; }
const std::string& AgendaModel::getFin() const { return fin; }
const std::string& AgendaModel::getTitulo() const { return titulo; }
const std::string& AgendaModel::getDetalle() const { return detalle; }
const std::string& AgendaModel::getTipo() const { return tipo; }
const std::vector<std::string>& AgendaModel::getResponsables() const { return responsables; }
void AgendaModel::setId(const std::string& value) { Entity<std::string>::setId(value); }
void AgendaModel::setDia(const std::string& value) { dia = value; }
void AgendaModel::setInicio(const std::string& value) { inicio = value; }
void AgendaModel::setFin(const std::string& value) { fin = value; }
void AgendaModel::setTitulo(const std::string& value) { titulo = value; }
void AgendaModel::setDetalle(const std::string& value) { detalle = value; }
void AgendaModel::setTipo(const std::string& value) { tipo = value; }
void AgendaModel::setResponsables(const std::vector<std::string>& value) { responsables = value; }
