#pragma once
#include "Generics/Service.h"
#include "Equipo/Models/EquipoModel.h"
#include "Equipo/Repositories/EquipoRepository.h"

class EquipoService : public Service<EquipoModel, EquipoRepository> {

public:

    explicit EquipoService(EquipoRepository& repo);

    std::vector<EquipoModel> findAll() const override;
    EquipoModel findById(int id) const override;
    int insert(const EquipoModel& entity) override;
    bool update(const EquipoModel& entity) override;
    bool remove(int id) override;

    bool validate(const EquipoModel& entity) override;
};
