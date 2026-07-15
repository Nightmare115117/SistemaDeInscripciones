#pragma once
#include "Generics/Service.h"
#include "Problematica/Model/ProblemaModel.h"
#include "Problematica/Repositories/ProblemaRepository.h"

class ProblemaService : public Service<ProblemaModel, ProblemaRepository> {

public:

    explicit ProblemaService(ProblemaRepository& repo);

    std::vector<ProblemaModel> findAll() const override;
    ProblemaModel findById(int id) const override;
    int insert(const ProblemaModel& entity) override;
    bool update(const ProblemaModel& entity) override;
    bool remove(int id) override;

    bool validate(const ProblemaModel& entity) override;
};
