#pragma once
#include "DBConfig/DBConfig.h"
#include "Generics/Repository.h"
#include "Registro/Models/RegistroModel.h"

class RegistroRepository : public Repository <RegistroModel> {
    
    DBConfig& dbConfig;

public:

    explicit RegistroRepository(DBConfig& dbConfig);
    
    std::vector<RegistroModel> findAll () const override;
    RegistroModel findById (int id) const override;
    int insert (const RegistroModel& entity) override;
    bool update (const RegistroModel& entity) override;
    bool remove (int id) override;
};