#include "AdminController.h"
#include <stdexcept>

AdminController::AdminController(AdminService& service)
    : Controller<AdminModel, AdminService>(service) {}

crow::json::wvalue AdminController::toJson(const AdminModel& entidad) const {
    crow::json::wvalue json;
    json["id"] = entidad.getId();
    json["nombre"] = entidad.getNombre();
    json["correo"] = entidad.getCorreo();
    json["contrasena"] = entidad.getContrasena();
    return json;
}

AdminModel AdminController::fromJson(const crow::json::rvalue& json) const {
    return AdminModel(json["nombre"].s(), json["correo"].s(), json["contrasena"].s());
}