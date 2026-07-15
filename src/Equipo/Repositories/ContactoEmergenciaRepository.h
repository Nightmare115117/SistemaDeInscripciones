#pragma once
#include "DBConfig/DBConfig.h"
#include "Generics/Repository.h"
#include "Equipo/Models/ContactoEmergenciaModel.h"

class ContactoEmergenciaRepository : public Repository<ContactoEmergenciaModel> {

    DBConfig& dbConfig;

public:

    explicit ContactoEmergenciaRepository(DBConfig& dbConfig);

    std::vector<ContactoEmergenciaModel> findAll() const override;
    ContactoEmergenciaModel findById(int id) const override;
    int insert(const ContactoEmergenciaModel& entity) override;
    bool update(const ContactoEmergenciaModel& entity) override;
    bool remove(int id) override;
};
