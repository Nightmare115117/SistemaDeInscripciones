#pragma once
#include "Evento/Patrocinadores/PatrocinadorModel.h"
#include "DBConfig/DBConfig.h"
#include <vector>

class PatrocinadorRepository {
    DBConfig& dbConfig;
public:
    explicit PatrocinadorRepository(DBConfig& config) : dbConfig(config) {}
    std::vector<PatrocinadorModel> findAll() const;
    PatrocinadorModel insert(const PatrocinadorModel& item) const;
    PatrocinadorModel update(const std::string& id, const PatrocinadorModel& item) const;
    bool remove(const std::string& id) const;
};
