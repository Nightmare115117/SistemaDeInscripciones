#pragma once
#include "Evento/Patrocinadores/PatrocinadorModel.h"
#include "Evento/Repositories/PatrocinadorRepository.h"

class PatrocinadorService {
    PatrocinadorRepository& repo;
public:
    explicit PatrocinadorService(PatrocinadorRepository& repository);
    std::vector<PatrocinadorModel> findAll() const;
    PatrocinadorModel insert(const PatrocinadorModel& item) const;
    PatrocinadorModel update(const std::string& id, const PatrocinadorModel& item) const;
    bool remove(const std::string& id) const;
private:
    static void validate(const PatrocinadorModel& item);
    static void validateId(const std::string& id);
};
