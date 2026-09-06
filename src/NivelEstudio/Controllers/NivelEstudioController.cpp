#include "NivelEstudioController.h"
#include <stdexcept>

NivelEstudioController::NivelEstudioController(NivelEstudioService& service)
    : Controller<NivelEstudioModel, NivelEstudioService>(service) {}

crow::json::wvalue NivelEstudioController::toJson(const NivelEstudioModel& entity) const {
    crow::json::wvalue json;
    json["id"] = entity.getId();
    json["nivel"] = entity.getNivel();
    return json;
}

NivelEstudioModel NivelEstudioController::fromJson(const crow::json::rvalue& json) const {
    if (!json.has("nivel")) {
        throw std::invalid_argument("Falta el campo 'nivel'");
    }
    return NivelEstudioModel(json["nivel"].s());
}
