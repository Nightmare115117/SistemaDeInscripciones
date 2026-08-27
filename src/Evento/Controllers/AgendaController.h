#pragma once
#include "Evento/Services/AgendaService.h"
#include <stdexcept>

class AgendaController {
    AgendaService& service;
    static AgendaModel fromJson(const crow::json::rvalue& body) {
        AgendaModel item;
        if (!body.has("dia") || !body.has("inicio") || !body.has("fin") || !body.has("titulo")) throw std::invalid_argument("Faltan datos de agenda");
        item.setDia(body["dia"].s()); item.setInicio(body["inicio"].s()); item.setFin(body["fin"].s()); item.setTitulo(body["titulo"].s());
        if (body.has("detalle") && body["detalle"].t() != crow::json::type::Null) item.setDetalle(body["detalle"].s());
        if (body.has("tipo")) item.setTipo(body["tipo"].s());
        if (body.has("responsables")) { std::vector<std::string> ids; for (const auto& id : body["responsables"]) ids.push_back(id.s()); item.setResponsables(ids); }
        return item;
    }
    static crow::json::wvalue json(const AgendaModel& item) {
        crow::json::wvalue out; out["id"] = item.getId(); out["dia"] = item.getDia(); out["inicio"] = item.getInicio(); out["fin"] = item.getFin(); out["titulo"] = item.getTitulo(); out["detalle"] = item.getDetalle(); out["tipo"] = item.getTipo();
        std::vector<crow::json::wvalue> ids; for (const auto& id : item.getResponsables()) { crow::json::wvalue value; value = id; ids.push_back(std::move(value)); } out["responsables"] = std::move(ids); return out;
    }
public:
    explicit AgendaController(AgendaService& itemService) : service(itemService) {}
    template <typename AppType> void registrarRutas(AppType& app) {
        CROW_ROUTE(app, "/api/agenda").methods(crow::HTTPMethod::GET)([this] { std::vector<crow::json::wvalue> out; for (const auto& item : service.findAll()) out.push_back(json(item)); crow::json::wvalue result; result = std::move(out); return crow::response(200, result); });
        CROW_ROUTE(app, "/api/agenda").methods(crow::HTTPMethod::POST)([this](const crow::request& req) { try { auto item = service.insert(fromJson(crow::json::load(req.body))); return crow::response(201, json(item)); } catch (const std::exception& e) { crow::json::wvalue error; error["detail"] = e.what(); return crow::response(422, error); } });
        CROW_ROUTE(app, "/api/agenda/<string>").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req, std::string id) { try { auto item = service.update(id, fromJson(crow::json::load(req.body))); return crow::response(200, json(item)); } catch (const std::exception& e) { crow::json::wvalue error; error["detail"] = e.what(); return crow::response(422, error); } });
        CROW_ROUTE(app, "/api/agenda/<string>").methods(crow::HTTPMethod::DELETE)([this](std::string id) { return service.remove(id) ? crow::response(204) : crow::response(404); });
    }
};
