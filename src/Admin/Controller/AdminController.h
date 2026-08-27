#pragma once

#include "Generics/Controller.h"
#include "Utils/JWT.h"
#include "Middleware/AuthMiddleware.h"
#include "Admin/Model/AdminModel.h"
#include "Admin/Service/AdminService.h"

class AdminController : public Controller<AdminModel, AdminService> {
    
public:

    explicit AdminController(AdminService& service);

    crow::json::wvalue toJson(const AdminModel& entity) const override;
    AdminModel fromJson(const crow::json::rvalue& json) const override;

    template <typename AppType>
    void HacerInicioDeSesion (AppType& app, const AdminService& service) {
        CROW_ROUTE(app, "/api/login")
        .methods(crow::HTTPMethod::POST)([&service](const crow::request& req) {
            try {
                auto body = crow::json::load(req.body);
                if (!body) {
                    crow::json::wvalue res;
                    res["error"] = "JSON inválido";
                    return crow::response(400, res);
                }

                AdminModel entity;
                entity.setCorreo(body.has("usuario") ? body["usuario"].s() : body["correo"].s());
                entity.setContrasena(body.has("password") ? body["password"].s() : body["contrasena"].s());

                AdminModel admin;
                admin = service.login(entity);

                std::string resultado = JWT::generarToken(admin.getId(), admin.getNombre());
                std::string usuario = body.has("usuario") ? std::string(body["usuario"].s()) : std::string(body["correo"].s());

                crow::json::wvalue res;
                res["token"] = resultado;
                res["usuario"] = usuario;
                res["nombre"] = admin.getNombre();
                res["rol"] = "admin";
                res["expiraEn"] = "24h";
                return crow::response(200, res);
            } catch (const std::logic_error& e) {
                crow::json::wvalue res;
                res["detail"] = e.what();
                return crow::response(401, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["detail"] = e.what();
                return crow::response(500, res);
            }
        });
    }

    template <typename AppType>
    void quienSoy(AppType& app, const AdminService& service) {
        CROW_ROUTE(app, "/api/yo")
        .methods(crow::HTTPMethod::GET)([&service](const crow::request& req) {
            const auto& auth = *static_cast<const AuthMiddleware::context*>(req.middleware_context);
            try {
                auto admin = service.findById(auth.adminId);
                crow::json::wvalue res;
                res["id"] = std::to_string(admin.getId());
                res["usuario"] = auth.username;
                res["nombre"] = admin.getNombre();
                res["rol"] = auth.role;
                return crow::response(200, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["detail"] = e.what();
                return crow::response(404, res);
            }
        });
    }
};