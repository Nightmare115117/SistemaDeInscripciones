#include "RegistroController.h"
#include "crow.h"
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace crow;

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

void RegistroController::contadorDeRegistros(SimpleApp& app, const RegistroService& service) {

    CROW_ROUTE(app, "/api/registro/count")([&service]() {
        try {
            json::wvalue res;
            res["total"] = service.countById();
            return response(200, res);
        } catch(const exception& e) {
            json::wvalue res;
            res["Error"] = e.what();
            return response(500, res);
        }
    });

}