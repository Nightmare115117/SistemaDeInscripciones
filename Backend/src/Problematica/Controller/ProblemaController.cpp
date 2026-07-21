#include "ProblemaController.h"
#include <stdexcept>

ProblemaController::ProblemaController(ProblemaService& service)
    : Controller<ProblemaModel, ProblemaService>(service) {}

crow::json::wvalue ProblemaController::toJson(const ProblemaModel& entidad) const {
    crow::json::wvalue json;
    json["id"] = entidad.getId();
    json["nombre"] = entidad.getNombre();
    json["descripcion"] = entidad.getDescripcion();
    return json;
}

ProblemaModel ProblemaController::fromJson(const crow::json::rvalue& json) const {
    if (!json.has("nombre")) {
        throw std::invalid_argument("Falta el campo 'nombre'");
    }
    if (!json.has("descripcion")) {
        throw std::invalid_argument("Falta el campo 'descripcion'");
    }

    return ProblemaModel(json["nombre"].s(), json["descripcion"].s());
}
