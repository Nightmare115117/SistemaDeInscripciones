#include "AlumnoService.h"
#include "Security/Crypto.h"
#include <stdexcept>

using namespace std;

AlumnoService::AlumnoService(AlumnoRepository& repo)
    : Service<AlumnoModel, AlumnoRepository>(repo) {}

bool AlumnoService::validate(const AlumnoModel& entity) {
    if (entity.getIdEquipo() <= 0 && entity.getIdEquipo() != -1) return false;
    if (entity.getIdContacto() <= 0 && entity.getIdContacto() != -1) return false;
    if (entity.getIdUniversidad() <= 0 && entity.getIdUniversidad() != -1) return false;
    return true;
}

vector<AlumnoModel> AlumnoService::findAll() const {
    vector<AlumnoModel> decode;
    for (AlumnoModel entity : repo.findAll()) {
        entity.setCorreo(AES::decrypt(entity.getCorreo()));
        entity.setNumeroTel(AES::decrypt(entity.getNumeroTel()));
        decode.push_back(entity);
    }
    return decode;
}

AlumnoModel AlumnoService::findById(int id) const {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    AlumnoModel alumno = repo.findById(id);
    alumno.setCorreo(AES::decrypt(alumno.getCorreo()));
    alumno.setNumeroTel(AES::decrypt(alumno.getNumeroTel()));
    return alumno;
}

int AlumnoService::insert(const AlumnoModel& entity) {
    if (!validate(entity)) throw logic_error("Los datos del alumno no son válidos");
    if (entity.getIdEquipo() > 0) {
        int cantidad = repo.countByIdOrderByGroup(entity.getIdEquipo()).Cantidad;
        if (cantidad >= 6 || cantidad <= 2) 
            throw runtime_error("Error la cantidad de integrantes en el Equipo es mayor a 5 o es menor de 3");
    }
    AlumnoModel entityC = entity;
    entityC.setCorreo(AES::encrypt(entityC.getCorreo()));
    entityC.setNumeroTel(AES::encrypt(entityC.getNumeroTel()));
    return repo.insert(entityC);
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
