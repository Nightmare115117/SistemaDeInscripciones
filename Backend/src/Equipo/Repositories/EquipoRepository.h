#pragma once
#include "Generics/Repository.h"
#include "Equipo/Models/EquipoModel.h"
#include "DBConfig/DBConfig.h"

class EquipoRepository : public Repository<EquipoModel> {

    DBConfig& dbConfig;

public:

    explicit EquipoRepository(DBConfig& dbConfig);

    std::vector<EquipoModel> findAll() const override;
    EquipoModel findById(int id) const override;
    int insert(const EquipoModel& entity) override;
    bool update(const EquipoModel& entity) override;
    bool remove(int id) override;
};
