#pragma once
#include "Evento/Patrocinadores/PatrocinadorModel.h"
#include "Generics/Repository.h"
#include "DBConfig/DBConfig.h"
#include <vector>

class PatrocinadorRepository : public Repository<PatrocinadorModel, std::string> {
    DBConfig& dbConfig;
public:
    explicit PatrocinadorRepository(DBConfig& config) : dbConfig(config) {}
    std::vector<PatrocinadorModel> findAll() const override;
    PatrocinadorModel findById(const std::string& id) const override;
    PatrocinadorModel insert(const PatrocinadorModel& item) override;
    PatrocinadorModel update(const std::string& id, const PatrocinadorModel& item) const;
    bool update(const PatrocinadorModel& item) override;
    bool remove(const std::string& id) override;
    PatrocinadorModel updateAndFetch(const std::string& id, const PatrocinadorModel& item) const;
};
