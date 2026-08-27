#pragma once
#include "Evento/Agenda/AgendaModel.h"
#include "Evento/Repositories/AgendaRepository.h"
#include "Generics/Service.h"
#include <stdexcept>

class AgendaService : public Service<AgendaModel, AgendaRepository, std::string> {
public:
    explicit AgendaService(AgendaRepository& repository);
    std::vector<AgendaModel> findAll() const override;
    AgendaModel findById(const std::string& id) const override;
    AgendaModel insert(const AgendaModel& item) override;
    bool update(const AgendaModel& item) override;
    bool remove(const std::string& id) override;
    AgendaModel updateAndFetch(const std::string& id, const AgendaModel& item) const;
private:
    static void validate(const AgendaModel& item);
    static void validateId(const std::string& id);
};
