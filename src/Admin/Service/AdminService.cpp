#include "AdminService.h"
#include <stdexcept>

using namespace std;

AdminService::AdminService(AdminRepo& repo)
    : Service<AdminModel, AdminRepo>(repo) {}

bool AdminService::validate(const AdminModel& entity) {
    if (entity.getNombre().empty()) return false;
    if (entity.getCorreo().empty()) return false;
    if (entity.getContrasena().empty()) return false;
    return true;
}

vector<AdminModel> AdminService::findAll() const {
    return repo.findAll();
}

AdminModel AdminService::findById(int id) const {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.findById(id);
}

int AdminService::insert(const AdminModel& entity) {
    if (!validate(entity)) throw logic_error("Los datos del Admin son equivocados");

    return repo.insert(entity);
}

bool AdminService::update(const AdminModel& entity) {
    if (entity.getId() <= 0) throw logic_error("El id debe ser positivo");
    if (!validate(entity)) throw logic_error("Los datos del admin no son válidos");

    return repo.update(entity);
}

bool AdminService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.remove(id);
}