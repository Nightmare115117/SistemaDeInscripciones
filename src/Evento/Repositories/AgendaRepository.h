#pragma once
#include "Evento/Agenda/AgendaModel.h"
#include "DBConfig/DBConfig.h"
#include <vector>

class AgendaRepository {
    DBConfig& dbConfig;
public:
    explicit AgendaRepository(DBConfig& config) : dbConfig(config) {}
    std::vector<AgendaModel> findAll() const;
    AgendaModel insert(const AgendaModel& agenda) const;
    AgendaModel update(const std::string& id, const AgendaModel& agenda) const;
    bool remove(const std::string& id) const;
private:
    AgendaModel hydrate(const std::string& id) const;
    DBConfig& config() const { return dbConfig; }
};
