#pragma once
#include "Generics/Controller.h"
#include "Equipo/Models/AlumnoModel.h"
#include "Equipo/Services/AlumnoService.h"

class AlumnoController : public Controller<AlumnoModel, AlumnoService> {

public:
    explicit AlumnoController(AlumnoService& service);

    crow::json::wvalue toJson(const AlumnoModel& entidad) const override;
    AlumnoModel fromJson(const crow::json::rvalue& json) const override;
    crow::json::wvalue toJsonDTO(const AlumnoRepository::EquipoCantidadDTO& entidad) const;

    template <typename AppType>
    void listaDeEquiposNombreYCantidadDeMiembros(AppType& app, const AlumnoService& service) {
        CROW_ROUTE(app, "/api/alumno/count/<int>")([this, &service](int id) {
            try {
                auto entidad = service.countByIdOrderByGroup(id);
                return crow::response(200, toJsonDTO(entidad));
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["Error"] = e.what();
                return crow::response(500, res);
            }
        });

        CROW_ROUTE(app, "/api/alumno/count")([this, &service]() {
            try {
                auto lista = service.countByIdOrderByGroupAsList();
                std::vector<crow::json::wvalue> jsonRes;
                for (const auto& item : lista) {
                    jsonRes.push_back(toJsonDTO(item));
                }
                crow::json::wvalue res;
                res = std::move(jsonRes);
                return crow::response(200, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["Error"] = "ocurrio un error intente más tarde";
                return crow::response(401, res);
            }
        });
    }
};
