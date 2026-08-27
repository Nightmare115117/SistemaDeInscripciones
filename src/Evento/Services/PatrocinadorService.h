#pragma once
#include "Evento/Patrocinadores/PatrocinadorModel.h"
#include "Evento/Repositories/PatrocinadorRepository.h"
#include "Generics/Service.h"

class PatrocinadorService : public Service<PatrocinadorModel, PatrocinadorRepository, std::string> {
public:
    explicit PatrocinadorService(PatrocinadorRepository& repository);
    std::vector<PatrocinadorModel> findAll() const override;
    PatrocinadorModel findById(const std::string& id) const override;
    PatrocinadorModel insert(const PatrocinadorModel& item) override;
    bool update(const PatrocinadorModel& item) override;
    bool remove(const std::string& id) override;
    PatrocinadorModel updateAndFetch(const std::string& id, const PatrocinadorModel& item) const;
private:
    static void validate(const PatrocinadorModel& item);
    static void validateId(const std::string& id);
};
