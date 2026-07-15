#include "EquipoService.h"
#include <stdexcept>

using namespace std;

EquipoService::EquipoService(EquipoRepository& repo)
    : Service<EquipoModel, EquipoRepository>(repo) {}

bool EquipoService::validate(const EquipoModel& entity) {
    if (entity.getNombre().empty()) return false;
    if (entity.getIdUniversidad() <= 0) return false;
    if (entity.getIdLider() <= 0) return false;
    if (entity.getIdProblematica() <= 0) return false;
    return true;
}

vector<EquipoModel> EquipoService::findAll() const {
    return repo.findAll();
}

EquipoModel EquipoService::findById(int id) const {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.findById(id);
}

int EquipoService::insert(const EquipoModel& entity) {
    if (!validate(entity)) throw logic_error("Los datos del equipo no son válidos");
    return repo.insert(entity);
}

bool EquipoService::update(const EquipoModel& entity) {
    if (entity.getId() <= 0) throw logic_error("El id debe ser positivo");
    if (!validate(entity)) throw logic_error("Los datos del equipo no son válidos");
    return repo.update(entity);
}

bool EquipoService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.remove(id);
}
