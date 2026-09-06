#include "PaisController.h"
#include <stdexcept>

PaisController::PaisController(PaisService& service)
    : Controller<PaisModel, PaisService>(service) {}

crow::json::wvalue PaisController::toJson(const PaisModel& entity) const {
    crow::json::wvalue json;
    json["id"] = entity.getId();
    json["nombre"] = entity.getNombre();
    json["codigoIso2"] = entity.getCodigoIso2();
    json["codigoIso3"] = entity.getCodigoIso3();
    return json;
}

PaisModel PaisController::fromJson(const crow::json::rvalue& json) const {
    if (!json.has("nombre") || !json.has("codigoIso2") || !json.has("codigoIso3")) {
        throw std::invalid_argument("Faltan campos obligatorios del pais");
    }
    return PaisModel(json["nombre"].s(), json["codigoIso2"].s(), json["codigoIso3"].s());
}
