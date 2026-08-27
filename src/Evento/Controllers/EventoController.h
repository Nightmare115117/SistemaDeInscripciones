#pragma once
#include "Evento/Services/EventoService.h"

class EventoController {
    EventoService& service;
    static crow::json::wvalue json(const EventoModel& item) {
        crow::json::wvalue out;
        out["cupo"] = item.getCupo();
        out["minIntegrantes"] = item.getMinIntegrantes();
        out["maxIntegrantes"] = item.getMaxIntegrantes();
        out["registroAbierto"] = item.getRegistroAbierto();
        std::vector<crow::json::wvalue> problems;
        for (const auto& problem : item.getProblematicas()) {
            crow::json::wvalue value;
            value["id"] = problem.id; value["code"] = problem.code;
            value["title"] = problem.title; value["description"] = problem.description;
            problems.push_back(std::move(value));
        }
        out["problematicas"] = std::move(problems);
        return out;
    }
public:
    explicit EventoController(EventoService& itemService) : service(itemService) {}
    template <typename AppType> void registrarRutas(AppType& app) {
        CROW_ROUTE(app, "/api/evento").methods(crow::HTTPMethod::GET)([this] { return crow::response(200, json(service.find())); });
        CROW_ROUTE(app, "/api/evento").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req) {
            try {
                auto body = crow::json::load(req.body); if (!body) throw std::invalid_argument("JSON invalido");
                EventoModel item = service.find();
                if (body.has("cupo")) item.setCupo(body["cupo"].i());
                if (body.has("minIntegrantes")) item.setMinIntegrantes(body["minIntegrantes"].i());
                if (body.has("maxIntegrantes")) item.setMaxIntegrantes(body["maxIntegrantes"].i());
                if (body.has("registroAbierto")) item.setRegistroAbierto(body["registroAbierto"].b());
                if (item.getCupo() < 1 || item.getMinIntegrantes() < 1 || item.getMaxIntegrantes() < item.getMinIntegrantes())
                    throw std::invalid_argument("Configuracion de evento invalida");
                return crow::response(200, json(service.update(item)));
            } catch (const std::exception& e) { crow::json::wvalue error; error["detail"] = e.what(); return crow::response(422, error); }
        });
        CROW_ROUTE(app, "/api/correos/estado").methods(crow::HTTPMethod::GET)([this] {
            auto item = service.correoEstado(); crow::json::wvalue out;
            out["configurado"] = item.getConfigurado(); out["habilitado"] = item.getHabilitado(); out["remitente"] = item.getRemitente();
            return crow::response(200, out);
        });
    }
};
