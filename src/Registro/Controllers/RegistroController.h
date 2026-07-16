#pragma once
#include "Generics/Controller.h"
#include "Registro/Models/RegistroModel.h"
#include "Registro/Services/RegistroService.h"

class RegistroController : public Controller <RegistroModel, RegistroService> {

public:
    explicit RegistroController(RegistroService& service);

    crow::json::wvalue toJson(const RegistroModel& entidad) const override;
    RegistroModel fromJson(const crow::json::rvalue& json) const override;

    template <typename AppType>
    void contadorDeRegistros(AppType& app, const RegistroService& service) {
        CROW_ROUTE(app, "/api/registro/count")([&service]() {
            try {
                crow::json::wvalue res;
                res["total"] = service.countById();
                return crow::response(200, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["Error"] = e.what();
                return crow::response(500, res);
            }
        });
    }
};