#include "AlumnoService.h"
#include <stdexcept>

using namespace std;

AlumnoService::AlumnoService(AlumnoRepository& repo)
    : Service<AlumnoModel, AlumnoRepository>(repo) {}

bool AlumnoService::validate(const AlumnoModel& entity) {
    if (entity.getNombre().empty()) return false;
    if (entity.getIdEquipo() <= 0) return false;
    if (entity.getCorreo().empty()) return false;
    return true;
}

vector<AlumnoModel> AlumnoService::findAll() const {
    return repo.findAll();
}

AlumnoModel AlumnoService::findById(int id) const {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.findById(id);
}

int AlumnoService::insert(const AlumnoModel& entity) {
    if (!validate(entity)) throw logic_error("Los datos del alumno no son válidos");
    return repo.insert(entity);
}

bool AlumnoService::update(const AlumnoModel& entity) {
    if (entity.getId() <= 0) throw logic_error("El id debe ser positivo");
    if (!validate(entity)) throw logic_error("Los datos del alumno no son válidos");
    return repo.update(entity);
}

bool AlumnoService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.remove(id);
}
