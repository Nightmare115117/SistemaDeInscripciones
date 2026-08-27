#include "EventoModel.h"

int EventoModel::getCupo() const { return cupo; }
int EventoModel::getMinIntegrantes() const { return minIntegrantes; }
int EventoModel::getMaxIntegrantes() const { return maxIntegrantes; }
bool EventoModel::getRegistroAbierto() const { return registroAbierto; }
const std::vector<ProblematicaDTO>& EventoModel::getProblematicas() const { return problematicas; }
void EventoModel::setCupo(int value) { cupo = value; }
void EventoModel::setMinIntegrantes(int value) { minIntegrantes = value; }
void EventoModel::setMaxIntegrantes(int value) { maxIntegrantes = value; }
void EventoModel::setRegistroAbierto(bool value) { registroAbierto = value; }
void EventoModel::setProblematicas(const std::vector<ProblematicaDTO>& value) { problematicas = value; }
bool CorreoEstadoModel::getConfigurado() const { return configurado; }
bool CorreoEstadoModel::getHabilitado() const { return habilitado; }
const std::string& CorreoEstadoModel::getRemitente() const { return remitente; }
void CorreoEstadoModel::setConfigurado(bool value) { configurado = value; }
void CorreoEstadoModel::setHabilitado(bool value) { habilitado = value; }
void CorreoEstadoModel::setRemitente(const std::string& value) { remitente = value; }
