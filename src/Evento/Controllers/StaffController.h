#pragma once
#include "Evento/Services/StaffService.h"
#include <stdexcept>

class StaffController {
    StaffService& service;
    static StaffModel fromJson(const crow::json::rvalue& body) { StaffModel item; if (body.has("nombre")) item.setNombre(body["nombre"].s()); if (body.has("rol")) item.setRol(body["rol"].s()); if (body.has("area")) item.setArea(body["area"].s()); if (body.has("correo")) item.setCorreo(body["correo"].s()); if (body.has("telefono")) item.setTelefono(body["telefono"].s()); return item; }
    static crow::json::wvalue json(const StaffModel& item) { crow::json::wvalue out; out["id"] = item.getId(); out["nombre"] = item.getNombre(); out["rol"] = item.getRol(); out["area"] = item.getArea(); out["correo"] = item.getCorreo(); out["telefono"] = item.getTelefono(); return out; }
public:
    explicit StaffController(StaffService& itemService) : service(itemService) {}
    template <typename AppType> void registrarRutas(AppType& app) {
        CROW_ROUTE(app, "/api/staff").methods(crow::HTTPMethod::GET)([this] { std::vector<crow::json::wvalue> out; for (const auto& item : service.findAll()) out.push_back(json(item)); crow::json::wvalue result; result = std::move(out); return crow::response(200, result); });
        CROW_ROUTE(app, "/api/staff").methods(crow::HTTPMethod::POST)([this](const crow::request& req) { try { auto item = service.insert(fromJson(crow::json::load(req.body))); return crow::response(201, json(item)); } catch (const std::exception& e) { crow::json::wvalue error; error["detail"] = e.what(); return crow::response(422, error); } });
        CROW_ROUTE(app, "/api/staff/<string>").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req, std::string id) { try { return crow::response(200, json(service.update(id, fromJson(crow::json::load(req.body))))); } catch (const std::exception& e) { crow::json::wvalue error; error["detail"] = e.what(); return crow::response(422, error); } });
        CROW_ROUTE(app, "/api/staff/<string>").methods(crow::HTTPMethod::DELETE)([this](std::string id) { return service.remove(id) ? crow::response(204) : crow::response(404); });
    }
};
