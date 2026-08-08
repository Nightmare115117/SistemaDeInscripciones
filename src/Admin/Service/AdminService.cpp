#include "AdminService.h"
#include "Security/Crypto.h"
#include "Security/HMAC.h"
#include <stdexcept>

using namespace std;

AdminService::AdminService(AdminRepo& repo, PasswordHasher& hasher)
    : Service<AdminModel, AdminRepo>(repo), hasher(hasher) {}

bool AdminService::validate(const AdminModel& entity) {
    if (entity.getCorreo().empty()) return false;
    if (entity.getContrasena().empty()) return false;
    if (entity.getNombre().empty()) return false;
    return true;
}

vector<AdminModel> AdminService::findAll() const {
    vector<AdminModel> decifrados;
    for (AdminModel entity : repo.findAll()) {
        entity.setCorreo(AES::decrypt(entity.getCorreo()));
        decifrados.push_back(entity);
    }
    return decifrados;
}

AdminModel AdminService::findById(int id) const {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    AdminModel admin = repo.findById(id);
    admin.setCorreo(AES::decrypt(admin.getCorreo()));
    return admin;
}

int AdminService::insert(const AdminModel& entity) {
    if (!validate(entity)) throw logic_error("Los datos del Admin son equivocados");

    AdminModel entityC = entity;
    entityC.setContrasena(hasher.hash(entityC.getContrasena()));
    entityC.setCorreoAES(AES::encrypt(entityC.getCorreo()));
    entityC.setCorreoHMAC(HMACsecurity::generate(entityC.getCorreo()));
    return repo.insert(entityC);
}

bool AdminService::update(const AdminModel& entity) {
    if (entity.getId() <= 0) throw logic_error("El id debe ser positivo");
    AdminModel copia = entity;
    if (!entity.getCorreo().empty()) {
        copia.setCorreoAES(AES::encrypt(copia.getCorreo()));
        copia.setCorreoHMAC(HMACsecurity::generate(copia.getCorreo()));
    }

    if (!copia.getContrasena().empty()) {
        copia.setContrasena(hasher.hash(copia.getContrasena()));
    }

    return repo.update(copia);
}

bool AdminService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");
    return repo.remove(id);
}

AdminModel AdminService::login(const AdminModel& entity) const{
    if (entity.getCorreo().empty()) 
        throw logic_error("El correo no puede estar vacio");
    
    if(entity.getContrasena().empty())
        throw logic_error("La contraseña no puede estar vacia");

    AdminModel admin = repo.findByCorreoHMAC(HMACsecurity::generate(entity.getCorreo()));

    if (AES::decrypt(admin.getCorreoAES()) != entity.getCorreo())
        throw logic_error("Credencieles incorrectas");

    if (!hasher.verificar(entity.getContrasena(), admin.getContrasena()))
        throw logic_error("Credencieles incorrectas");
        
    return admin;
}