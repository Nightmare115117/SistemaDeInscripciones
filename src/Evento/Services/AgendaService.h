#pragma once
#include "Evento/Agenda/AgendaModel.h"
#include "Evento/Repositories/AgendaRepository.h"
#include <stdexcept>

class AgendaService {
    AgendaRepository& repo;
public:
    explicit AgendaService(AgendaRepository& repository);
    std::vector<AgendaModel> findAll() const;
    AgendaModel insert(const AgendaModel& item) const;
    AgendaModel update(const std::string& id, const AgendaModel& item) const;
    bool remove(const std::string& id) const;
private:
    static void validate(const AgendaModel& item);
    static void validateId(const std::string& id);
};
