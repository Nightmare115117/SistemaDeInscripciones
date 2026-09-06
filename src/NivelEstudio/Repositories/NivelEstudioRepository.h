#pragma once
#include "DBConfig/DBConfig.h"
#include "Generics/Repository.h"
#include "NivelEstudio/Models/NivelEstudioModel.h"

class NivelEstudioRepository : public Repository<NivelEstudioModel> {
    DBConfig& dbConfig;

public:
    explicit NivelEstudioRepository(DBConfig& dbConfig);

    std::vector<NivelEstudioModel> findAll() const override;
    NivelEstudioModel findById(int id) const override;
    int insert(const NivelEstudioModel& entity) override;
    bool update(const NivelEstudioModel& entity) override;
    bool remove(int id) override;
};
