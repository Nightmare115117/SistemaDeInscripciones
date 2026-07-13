#include "RegistroController.h"

RegistroController::RegistroController(RegistroService& service)
    : Controller<RegistroModel, RegistroService>(service) {}

    crow::json::wvalue RegistroController::toJson(const RegistroModel& entidad) const {
    crow::json::wvalue json;
    json["id"] = entidad.getId();
    json["fecha-hora"] = entidad.getFechaHora();
    json["idEquipo"] = entidad.getIdEquipo();
    return json;
}

RegistroModel RegistroController::fromJson(const crow::json::rvalue& json) const {
    if (!json.has("idEquipo")) {
        throw std::invalid_argument("Falta el campo 'idEquipo'");
    }
    return RegistroModel(json["idEquipo"].i());
}