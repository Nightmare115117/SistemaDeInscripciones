#pragma once
#include "Evento/Evento/EventoModel.h"
#include "DBConfig/DBConfig.h"

class EventoRepository {
    DBConfig& dbConfig;
public:
    explicit EventoRepository(DBConfig& config) : dbConfig(config) {}
    EventoModel find() const;
    EventoModel update(const EventoModel& evento) const;
    CorreoEstadoModel correoEstado() const;
};
