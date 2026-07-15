#include "ContactoEmergenciaService.h"
#include <stdexcept>

using namespace std;

ContactoEmergenciaService::ContactoEmergenciaService(ContactoEmergenciaRepository& repo)
    : Service<ContactoEmergenciaModel, ContactoEmergenciaRepository>(repo) {}

bool ContactoEmergenciaService::validate(const ContactoEmergenciaModel& entity) {
    return !entity.getNombre().empty() && !entity.getTelefono().empty() && !entity.getPariente().empty();
}

vector<ContactoEmergenciaModel> ContactoEmergenciaService::findAll() const {
    return repo.findAll();
}

ContactoEmergenciaModel ContactoEmergenciaService::findById(int id) const {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.findById(id);
}

int ContactoEmergenciaService::insert(const ContactoEmergenciaModel& entity) {
    if (!validate(entity)) throw logic_error("Los datos del contacto de emergencia no son válidos");
    return repo.insert(entity);
}

bool ContactoEmergenciaService::update(const ContactoEmergenciaModel& entity) {
    if (entity.getId() <= 0) throw logic_error("El id debe ser positivo");
    if (!validate(entity)) throw logic_error("Los datos del contacto de emergencia no son válidos");
    return repo.update(entity);
}

bool ContactoEmergenciaService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.remove(id);
}
