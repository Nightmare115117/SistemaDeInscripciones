#include "UniversidadController.h"
#include <stdexcept>

UniversidadController::UniversidadController(UniversityService& service)
    : Controller<UniversityModel, UniversityService>(service) {}

crow::json::wvalue UniversidadController::toJson(const UniversityModel& entidad) const {
    crow::json::wvalue json;
    json["id"] = entidad.getId();
    json["nombre"] = entidad.getNombre();
    return json;
}

UniversityModel UniversidadController::fromJson(const crow::json::rvalue& json) const {
    if (!json.has("nombre")) {
        throw std::invalid_argument("Falta el campo 'nombre'");
    }
    return UniversityModel(json["nombre"].s());
}
