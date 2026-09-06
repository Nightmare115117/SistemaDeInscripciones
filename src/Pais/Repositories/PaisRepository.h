#pragma once
#include "DBConfig/DBConfig.h"
#include "Generics/Repository.h"
#include "Pais/Models/PaisModel.h"

class PaisRepository : public Repository<PaisModel> {
    DBConfig& dbConfig;

public:
    explicit PaisRepository(DBConfig& dbConfig);

    std::vector<PaisModel> findAll() const override;
    PaisModel findById(int id) const override;
    int insert(const PaisModel& entity) override;
    bool update(const PaisModel& entity) override;
    bool remove(int id) override;
};
