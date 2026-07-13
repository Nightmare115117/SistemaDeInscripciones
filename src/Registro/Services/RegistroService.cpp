#include "RegistroService.h"

using namespace std;

bool RegistroService::validate(const RegistroModel& entity) {
    if (entity.getIdEquipo() <= 0 && entity.getIdEquipo() != -1) return false;
    return true;
}

bool RegistroService::exist(int id) {
    if (repo.findById(id).getId() == -1) return false;
    return true;
}

vector<RegistroModel> RegistroService::findAll() const {
    return repo.findAll();
}