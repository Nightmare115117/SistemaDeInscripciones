#include "EquipoController.h"
#include <stdexcept>

EquipoController::EquipoController(EquipoService& service)
    : Controller<EquipoModel, EquipoService>(service) {}

crow::json::wvalue EquipoController::toJson(const EquipoModel& entidad) const {
    crow::json::wvalue json;
    json["id"] = entidad.getId();
    json["nombre"] = entidad.getNombre();
    json["idUniversidad"] = entidad.getIdUniversidad();
    json["idLider"] = entidad.getIdLider();
    json["idProblematica"] = entidad.getIdProblematica();
    return json;
}

EquipoModel EquipoController::fromJson(const crow::json::rvalue& json) const {
    if (!json.has("nombre")) {
        throw std::invalid_argument("Falta el campo 'nombre'");
    }
    if (!json.has("idUniversidad")) {
        throw std::invalid_argument("Falta el campo 'idUniversidad'");
    }
    if (!json.has("idLider")) {
        throw std::invalid_argument("Falta el campo 'idLider'");
    }
    if (!json.has("idProblematica")) {
        throw std::invalid_argument("Falta el campo 'idProblematica'");
    }

    EquipoModel equipo(
        json["nombre"].s(),
        json["idUniversidad"].i(),
        json["idLider"].i(),
        json["idProblematica"].i()
    );
    return equipo;
}
