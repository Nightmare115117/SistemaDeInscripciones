#pragma once
#include "Generics/Service.h"
#include "Pais/Models/PaisModel.h"
#include "Pais/Repositories/PaisRepository.h"

class PaisService : public Service<PaisModel, PaisRepository> {
public:
    explicit PaisService(PaisRepository& repo);

    std::vector<PaisModel> findAll() const override;
    PaisModel findById(int id) const override;
    int insert(const PaisModel& entity) override;
    bool update(const PaisModel& entity) override;
    bool remove(int id) override;
    bool validate(const PaisModel& entity) override;
};
