#pragma once
#include "Generics/Service.h"
#include "NivelEstudio/Models/NivelEstudioModel.h"
#include "NivelEstudio/Repositories/NivelEstudioRepository.h"

class NivelEstudioService : public Service<NivelEstudioModel, NivelEstudioRepository> {
public:
    explicit NivelEstudioService(NivelEstudioRepository& repo);

    std::vector<NivelEstudioModel> findAll() const override;
    NivelEstudioModel findById(int id) const override;
    int insert(const NivelEstudioModel& entity) override;
    bool update(const NivelEstudioModel& entity) override;
    bool remove(int id) override;
    bool validate(const NivelEstudioModel& entity) override;
};
