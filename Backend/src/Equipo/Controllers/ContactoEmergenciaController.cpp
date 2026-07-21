#include "ContactoEmergenciaController.h"
#include <stdexcept>

ContactoEmergenciaController::ContactoEmergenciaController(ContactoEmergenciaService& service)
    : Controller<ContactoEmergenciaModel, ContactoEmergenciaService>(service) {}

crow::json::wvalue ContactoEmergenciaController::toJson(const ContactoEmergenciaModel& entidad) const {
    crow::json::wvalue json;
    json["id"] = entidad.getId();
    json["nombre"] = entidad.getNombre();
    json["telefono"] = entidad.getTelefono();
    json["pariente"] = entidad.getPariente();
    return json;
}

ContactoEmergenciaModel ContactoEmergenciaController::fromJson(const crow::json::rvalue& json) const {
    if (!json.has("nombre")) {
        throw std::invalid_argument("Falta el campo 'nombre'");
    }
    if (!json.has("telefono")) {
        throw std::invalid_argument("Falta el campo 'telefono'");
    }
    if (!json.has("pariente")) {
        throw std::invalid_argument("Falta el campo 'pariente'");
    }

    return ContactoEmergenciaModel(
        json["nombre"].s(),
        json["telefono"].s(),
        json["pariente"].s()
    );
}
