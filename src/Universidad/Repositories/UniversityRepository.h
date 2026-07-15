#pragma once
#include "DBConfig/DBConfig.h"
#include "Generics/Repository.h"
#include "Universidad/Models/UniversityModel.h"

class UniversityRepository : public Repository<UniversityModel> {

    DBConfig& dbConfig;

public:

    explicit UniversityRepository(DBConfig& dbConfig);

    std::vector<UniversityModel> findAll() const override;
    UniversityModel findById(int id) const override;
    int insert(const UniversityModel& entity) override;
    bool update(const UniversityModel& entity) override;
    bool remove(int id) override;
};
