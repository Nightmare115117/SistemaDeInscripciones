#pragma once

#include "Generics/Controller.h"
#include "Utils/JWT.h"
#include "Admin/Model/AdminModel.h"
#include "Admin/Service/AdminService.h"

class AdminController : public Controller<AdminModel, AdminService> {
    
public:

    explicit AdminController(AdminService& service);

    crow::json::wvalue toJson(const AdminModel& entity) const override;
    AdminModel fromJson(const crow::json::rvalue& json) const override;

    template <typename AppType>
    void HacerInicioDeSesion (AppType& app, const AdminService& service) {
        CROW_ROUTE(app, "/api/admin/login")
        .methods(crow::HTTPMethod::PUT)([&service](const crow::request& req) {
            try {
                auto body = crow::json::load(req.body);
                if (!body) {
                    crow::json::wvalue res;
                    res["error"] = "JSON inválido";
                    return crow::response(400, res);
                }

                AdminModel entity;
                entity.setCorreo(body["correo"].s());
                entity.setContrasena(body["contrasena"].s());

                AdminModel admin;
                admin = service.login(entity);

                std::string resultado = JWT::generarToken(admin.getId(), admin.getNombre());

                crow::json::wvalue res;
                res["token"] = resultado;
                return crow::response(200, res);
            } catch (const std::logic_error& e) {
                crow::json::wvalue res;
                res["Error"] = e.what();
                return crow::response(401, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["Error"] = e.what();
                return crow::response(500, res);
            }
        });
    }
};