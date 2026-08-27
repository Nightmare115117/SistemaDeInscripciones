#pragma once
#include "Generics/Controller.h"
#include "Equipo/Models/EquipoModel.h"
#include "Equipo/Services/EquipoService.h"
#include <stdexcept>

class EquipoController : public Controller<EquipoModel, EquipoService> {

public:
    explicit EquipoController(EquipoService& service);

    crow::json::wvalue toJson(const EquipoModel& entidad) const override;
    EquipoModel fromJson(const crow::json::rvalue& json) const override;

    template <typename AppType>
    void rutasAdministrativas(AppType& app) {
        CROW_ROUTE(app, "/api/equipos/<int>").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req, int id) {
            try {
                auto body = crow::json::load(req.body);
                if (!body) throw std::invalid_argument("JSON invalido");
                std::string estado;
                std::string revisadoEn;
                std::string notas;
                if (body.has("estado")) estado = body["estado"].s();
                if (body.has("revisadoEn")) revisadoEn = body["revisadoEn"].s();
                if (body.has("notas")) notas = body["notas"].s();
                if (!service.updateReview(id, estado, revisadoEn, notas)) return crow::response(404);
                return crow::response(200, toJson(service.findById(id)));
            } catch (const std::exception& e) { crow::json::wvalue error; error["detail"] = e.what(); return crow::response(422, error); }
        });
    }
};
