#include "ContactoEmergenciaService.h"
#include <stdexcept>
#include "Security/Crypto.h"

using namespace std;

ContactoEmergenciaService::ContactoEmergenciaService(ContactoEmergenciaRepository& repo)
    : Service<ContactoEmergenciaModel, ContactoEmergenciaRepository>(repo) {}

bool ContactoEmergenciaService::validate(const ContactoEmergenciaModel& entity) {
    return !entity.getNombre().empty() && !entity.getTelefono().empty() && !entity.getPariente().empty();
}

vector<ContactoEmergenciaModel> ContactoEmergenciaService::findAll() const {
    vector<ContactoEmergenciaModel> lista;
    for (ContactoEmergenciaModel entity : repo.findAll()) {
        entity.setTelefono(AES::decrypt(entity.getTelefono()));
        lista.push_back(entity);
    }
    return lista;
}

ContactoEmergenciaModel ContactoEmergenciaService::findById(int id) const {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    ContactoEmergenciaModel entity = repo.findById(id);
    entity.setTelefono(AES::decrypt(entity.getTelefono()));
    return entity;
}

int ContactoEmergenciaService::insert(const ContactoEmergenciaModel& entity) {
    if (!validate(entity)) throw logic_error("Los datos del contacto de emergencia no son válidos");
    ContactoEmergenciaModel entityC = entity;
    entityC.setTelefono(AES::encrypt(entityC.getTelefono()));
    return repo.insert(entityC);
}

bool ContactoEmergenciaService::update(const ContactoEmergenciaModel& entity) {
    if (entity.getId() <= 0) throw logic_error("El id debe ser positivo");
    if (!validate(entity)) throw logic_error("Los datos del contacto de emergencia no son válidos");
    ContactoEmergenciaModel entityC = entity;
    entityC.setTelefono(AES::encrypt(entityC.getTelefono()));
    return repo.update(entityC);
}

bool ContactoEmergenciaService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.remove(id);
}
