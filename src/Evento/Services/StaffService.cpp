#include "StaffService.h"
#include "Security/Crypto.h"
#include <stdexcept>

StaffService::StaffService(StaffRepository& repository) : repo(repository) {}
std::vector<StaffModel> StaffService::findAll() const {
    auto items = repo.findAll();
    for (auto& item : items) {
        item.setCorreo(AES::decrypt(item.getCorreo()));
        item.setTelefono(AES::decrypt(item.getTelefono()));
    }
    return items;
}

StaffModel StaffService::insert(const StaffModel& item) const {
    validate(item);
    StaffModel encrypted = item;
    encrypted.setCorreo(AES::encrypt(encrypted.getCorreo()));
    encrypted.setTelefono(AES::encrypt(encrypted.getTelefono()));
    StaffModel saved = repo.insert(encrypted);
    saved.setCorreo(AES::decrypt(saved.getCorreo()));
    saved.setTelefono(AES::decrypt(saved.getTelefono()));
    return saved;
}

StaffModel StaffService::update(const std::string& id, const StaffModel& item) const {
    validateId(id);
    StaffModel encrypted = item;
    if (!encrypted.getCorreo().empty()) encrypted.setCorreo(AES::encrypt(encrypted.getCorreo()));
    if (!encrypted.getTelefono().empty()) encrypted.setTelefono(AES::encrypt(encrypted.getTelefono()));
    StaffModel saved = repo.update(id, encrypted);
    saved.setCorreo(AES::decrypt(saved.getCorreo()));
    saved.setTelefono(AES::decrypt(saved.getTelefono()));
    return saved;
}
bool StaffService::remove(const std::string& id) const { return repo.remove(id); }
void StaffService::validate(const StaffModel& item) {
    if (item.getNombre().empty() || item.getRol().empty() || item.getCorreo().empty())
        throw std::invalid_argument("Staff requiere nombre, rol y correo");
}
void StaffService::validateId(const std::string& id) {
    if (id.empty()) throw std::invalid_argument("El id de staff no puede estar vacio");
}
