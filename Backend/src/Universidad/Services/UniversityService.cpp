#include "UniversityService.h"
#include <stdexcept>

using namespace std;

UniversityService::UniversityService(UniversityRepository& repo)
    : Service<UniversityModel, UniversityRepository>(repo) {}

bool UniversityService::validate(const UniversityModel& entity) {
    return !entity.getNombre().empty();
}

vector<UniversityModel> UniversityService::findAll() const {
    return repo.findAll();
}

UniversityModel UniversityService::findById(int id) const {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.findById(id);
}

int UniversityService::insert(const UniversityModel& entity) {
    if (!validate(entity)) throw logic_error("Los datos de la universidad no son válidos");
    return repo.insert(entity);
}

bool UniversityService::update(const UniversityModel& entity) {
    if (entity.getId() <= 0) throw logic_error("El id debe ser positivo");
    if (!validate(entity)) throw logic_error("Los datos de la universidad no son válidos");
    return repo.update(entity);
}

bool UniversityService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.remove(id);
}
