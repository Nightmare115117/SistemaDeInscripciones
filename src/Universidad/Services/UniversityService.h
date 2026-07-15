#pragma once
#include "Generics/Service.h"
#include "Universidad/Models/UniversityModel.h"
#include "Universidad/Repositories/UniversityRepository.h"

class UniversityService : public Service<UniversityModel, UniversityRepository> {

public:

    explicit UniversityService(UniversityRepository& repo);

    std::vector<UniversityModel> findAll() const override;
    UniversityModel findById(int id) const override;
    int insert(const UniversityModel& entity) override;
    bool update(const UniversityModel& entity) override;
    bool remove(int id) override;

    bool validate(const UniversityModel& entity) override;
};
