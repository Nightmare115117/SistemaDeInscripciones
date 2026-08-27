#include "EventoService.h"
#include <stdexcept>

EventoService::EventoService(EventoRepository& repository) : repo(repository) {}
EventoModel EventoService::find() const { return repo.find(); }
EventoModel EventoService::update(const EventoModel& item) const {
    validate(item);
    return repo.update(item);
}
CorreoEstadoModel EventoService::correoEstado() const { return repo.correoEstado(); }

void EventoService::validate(const EventoModel& item) {
    if (item.getCupo() < 1) throw std::invalid_argument("El cupo debe ser positivo");
    if (item.getMinIntegrantes() < 1 || item.getMaxIntegrantes() < item.getMinIntegrantes())
        throw std::invalid_argument("El rango de integrantes es invalido");
}
