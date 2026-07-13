#include "RegistroService.h"
#include <stdexcept>

using namespace std;

RegistroService::RegistroService(RegistroRepository& repo)
    : Service<RegistroModel, RegistroRepository>(repo) {}

bool RegistroService::validate(const RegistroModel& entity) {
    if (entity.getIdEquipo() <= 0 && entity.getIdEquipo() != -1) return false;
    return true;
}

vector<RegistroModel> RegistroService::findAll() const {
    return repo.findAll();
}

//throw runtime_error("Registro con el id: " + to_string(id) + " no existe");
//Usar en findById

RegistroModel RegistroService::findById(int id) const {
    if (id <= 0) throw logic_error("Id debe ser positivo");

    return repo.findById(id);
}

int RegistroService::insert(const RegistroModel& entity) {
    if (!validate(entity)) throw logic_error("El id no puede ser 0 ni negativo");

    return repo.insert(entity);
}

bool RegistroService::update(const RegistroModel& entity) {
    throw logic_error("No se pueden modificar registros existentes");
}

bool RegistroService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");

    return repo.remove(id);
}