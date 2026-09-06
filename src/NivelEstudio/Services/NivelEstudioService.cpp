#include "NivelEstudioService.h"
#include <stdexcept>

NivelEstudioService::NivelEstudioService(NivelEstudioRepository& repo)
    : Service<NivelEstudioModel, NivelEstudioRepository>(repo) {}

std::vector<NivelEstudioModel> NivelEstudioService::findAll() const {
    return repo.findAll();
}

NivelEstudioModel NivelEstudioService::findById(int id) const {
    if (id <= 0) throw std::logic_error("El id debe ser positivo");
    return repo.findById(id);
}

int NivelEstudioService::insert(const NivelEstudioModel& entity) {
    if (!validate(entity)) throw std::logic_error("El nivel de estudio es obligatorio");
    return repo.insert(entity);
}

bool NivelEstudioService::update(const NivelEstudioModel& entity) {
    if (entity.getId() <= 0) throw std::logic_error("El id debe ser positivo");
    if (!validate(entity)) throw std::logic_error("El nivel de estudio es obligatorio");
    return repo.update(entity);
}

bool NivelEstudioService::remove(int id) {
    if (id <= 0) throw std::logic_error("El id debe ser positivo");
    return repo.remove(id);
}

bool NivelEstudioService::validate(const NivelEstudioModel& entity) {
    return !entity.getNivel().empty();
}
