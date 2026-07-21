#include "AlumnoService.h"
#include <stdexcept>

using namespace std;

AlumnoService::AlumnoService(AlumnoRepository& repo)
    : Service<AlumnoModel, AlumnoRepository>(repo) {}

bool AlumnoService::validate(const AlumnoModel& entity) {
    if (entity.getNombre().empty()) return false;
    if (entity.getIdEquipo() <= 0) return false;
    if (!entity.firmoterminos()) return false;
    if (entity.getCorreo().empty()) return false;
    if (entity.getNumeroTel().empty()) return false;
    if (entity.getApellidoPaterno().empty()) return false;
    if (entity.getApellidoMaterno().empty()) return false;
    if (entity.getAlergias().empty()) return false;
    if (entity.getCondicionMedica().empty()) return false;
    if (entity.getMedicamento().empty()) return false;
    if (entity.getIdContacto() <= 0) return false;
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
    if (entity.getIdEquipo() > 0) {
        int cantidad = repo.countByIdOrderByGroup(entity.getIdEquipo()).Cantidad;
        if (cantidad >= 6 || cantidad <= 2) 
            throw runtime_error("Error la cantidad de integrantes en el Equipo es mayor a 5 o es menor de 3");
    }

    return repo.insert(entity);
}

bool AlumnoService::update(const AlumnoModel& entity) {
    if (entity.getId() <= 0) throw logic_error("El id debe ser positivo");
    if (!validate(entity)) throw logic_error("Los datos del alumno no son válidos");
    if (entity.getIdEquipo() > 0) {
        int cantidad = repo.countByIdOrderByGroup(entity.getIdEquipo()).Cantidad;
        if (cantidad >= 6 || cantidad <= 2) 
            throw runtime_error("Error la cantidad de integrantes en el Equipo es mayor a 5 o es menor de 3");
    }

    return repo.update(entity);
}

bool AlumnoService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.remove(id);
}

AlumnoRepository::EquipoCantidadDTO AlumnoService::countByIdOrderByGroup(int id) const{
    if (id <= 0)
        throw logic_error("El id es invalido");

    return repo.countByIdOrderByGroup(id);
}

std::vector<AlumnoRepository::EquipoCantidadDTO> AlumnoService::countByIdOrderByGroupAsList() const {
    return repo.countByIdOrderByGroupAsList();
}
