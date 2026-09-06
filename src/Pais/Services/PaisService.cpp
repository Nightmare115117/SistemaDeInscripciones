#include "PaisService.h"
#include <stdexcept>

PaisService::PaisService(PaisRepository& repo)
    : Service<PaisModel, PaisRepository>(repo) {}

std::vector<PaisModel> PaisService::findAll() const {
    return repo.findAll();
}

PaisModel PaisService::findById(int id) const {
    if (id <= 0) throw std::logic_error("El id debe ser positivo");
    return repo.findById(id);
}

int PaisService::insert(const PaisModel& entity) {
    if (!validate(entity)) throw std::logic_error("Nombre y codigos ISO del pais son obligatorios");
    return repo.insert(entity);
}

bool PaisService::update(const PaisModel& entity) {
    if (entity.getId() <= 0) throw std::logic_error("El id debe ser positivo");
    if (!validate(entity)) throw std::logic_error("Nombre y codigos ISO del pais son obligatorios");
    return repo.update(entity);
}

bool PaisService::remove(int id) {
    if (id <= 0) throw std::logic_error("El id debe ser positivo");
    return repo.remove(id);
}

bool PaisService::validate(const PaisModel& entity) {
    return !entity.getNombre().empty() && !entity.getCodigoIso2().empty() &&
        !entity.getCodigoIso3().empty();
}
