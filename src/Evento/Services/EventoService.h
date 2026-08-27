#pragma once
#include "Evento/Evento/EventoModel.h"
#include "Evento/Repositories/EventoRepository.h"

class EventoService {
    EventoRepository& repo;
public:
    explicit EventoService(EventoRepository& repository);
    EventoModel find() const;
    EventoModel update(const EventoModel& item) const;
    CorreoEstadoModel correoEstado() const;
private:
    static void validate(const EventoModel& item);
};
