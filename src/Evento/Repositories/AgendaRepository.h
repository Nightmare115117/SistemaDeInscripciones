#pragma once
#include "Evento/Agenda/AgendaModel.h"
#include "Generics/Repository.h"
#include "DBConfig/DBConfig.h"
#include <vector>

class AgendaRepository : public Repository<AgendaModel, std::string> {
    DBConfig& dbConfig;
public:
    explicit AgendaRepository(DBConfig& config) : dbConfig(config) {}
    std::vector<AgendaModel> findAll() const override;
    AgendaModel findById(const std::string& id) const override;
    AgendaModel insert(const AgendaModel& agenda) override;
    bool update(const AgendaModel& agenda) override;
    AgendaModel update(const std::string& id, const AgendaModel& agenda) const;
    bool remove(const std::string& id) override;
    AgendaModel updateAndFetch(const std::string& id, const AgendaModel& agenda) const;
private:
    AgendaModel hydrate(const std::string& id) const;
    DBConfig& config() const { return dbConfig; }
};
