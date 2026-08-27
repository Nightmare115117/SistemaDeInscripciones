#pragma once
#include "Evento/Services/PatrocinadorService.h"
#include "Generics/Controller.h"
#include <stdexcept>

class PatrocinadorController : public Controller<PatrocinadorModel, PatrocinadorService, std::string> {
    static PatrocinadorModel parse(const crow::json::rvalue& body) { PatrocinadorModel item; if (body.has("nombre")) item.setNombre(body["nombre"].s()); if (body.has("logo")) item.setLogo(body["logo"].s()); if (body.has("url")) item.setUrl(body["url"].s()); if (body.has("nivel")) item.setNivel(body["nivel"].s()); return item; }
    static crow::json::wvalue json(const PatrocinadorModel& item) { crow::json::wvalue out; out["id"] = item.getId(); out["nombre"] = item.getNombre(); out["logo"] = item.getLogo(); out["url"] = item.getUrl(); out["nivel"] = item.getNivel(); return out; }
public:
    explicit PatrocinadorController(PatrocinadorService& itemService) : Controller<PatrocinadorModel, PatrocinadorService, std::string>(itemService) {}
    crow::json::wvalue toJson(const PatrocinadorModel& item) const override { return json(item); }
    PatrocinadorModel fromJson(const crow::json::rvalue& body) const override { return parse(body); }
    template <typename AppType> void registrarRutas(AppType& app) {
        CROW_ROUTE(app, "/api/patrocinadores").methods(crow::HTTPMethod::GET)([this] { std::vector<crow::json::wvalue> out; for (const auto& item : service.findAll()) out.push_back(json(item)); crow::json::wvalue result; result = std::move(out); return crow::response(200, result); });
        CROW_ROUTE(app, "/api/patrocinadores").methods(crow::HTTPMethod::POST)([this](const crow::request& req) { try { return crow::response(201, json(service.insert(parse(crow::json::load(req.body))))); } catch (const std::exception& e) { crow::json::wvalue error; error["detail"] = e.what(); return crow::response(422, error); } });
        CROW_ROUTE(app, "/api/patrocinadores/<string>").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req, std::string id) { try { return crow::response(200, json(service.updateAndFetch(id, fromJson(crow::json::load(req.body))))); } catch (const std::exception& e) { crow::json::wvalue error; error["detail"] = e.what(); return crow::response(422, error); } });
        CROW_ROUTE(app, "/api/patrocinadores/<string>").methods(crow::HTTPMethod::DELETE)([this](std::string id) { return service.remove(id) ? crow::response(204) : crow::response(404); });
    }
};
