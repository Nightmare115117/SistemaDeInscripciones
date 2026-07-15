#include "ProblemaService.h"
#include <stdexcept>

using namespace std;

ProblemaService::ProblemaService(ProblemaRepository& repo)
    : Service<ProblemaModel, ProblemaRepository>(repo) {}

bool ProblemaService::validate(const ProblemaModel& entity) {
    return !entity.getNombre().empty() && !entity.getDescripcion().empty();
}

vector<ProblemaModel> ProblemaService::findAll() const {
    return repo.findAll();
}

ProblemaModel ProblemaService::findById(int id) const {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.findById(id);
}

int ProblemaService::insert(const ProblemaModel& entity) {
    if (!validate(entity)) throw logic_error("Los datos de la problematica no son válidos");
    return repo.insert(entity);
}

bool ProblemaService::update(const ProblemaModel& entity) {
    if (entity.getId() <= 0) throw logic_error("El id debe ser positivo");
    if (!validate(entity)) throw logic_error("Los datos de la problematica no son válidos");
    return repo.update(entity);
}

bool ProblemaService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.remove(id);
}
