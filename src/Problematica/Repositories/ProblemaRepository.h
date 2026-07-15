#pragma once
#include "DBConfig/DBConfig.h"
#include "Generics/Repository.h"
#include "Problematica/Model/ProblemaModel.h"

class ProblemaRepository : public Repository<ProblemaModel> {

    DBConfig& dbConfig;

public:

    explicit ProblemaRepository(DBConfig& dbConfig);

    std::vector<ProblemaModel> findAll() const override;
    ProblemaModel findById(int id) const override;
    int insert(const ProblemaModel& entity) override;
    bool update(const ProblemaModel& entity) override;
    bool remove(int id) override;
};
