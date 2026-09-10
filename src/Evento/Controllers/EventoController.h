#pragma once
#include "Evento/Services/EventoService.h"
#include "DBConfig/DBConfig.h"
#include <pqxx/pqxx>

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

        CROW_ROUTE(app, "/api/correo/estado").methods(crow::HTTPMethod::GET)([this]() {
            try {
                const auto estado = service.correoEstado();
                crow::json::wvalue res;
                res["configurado"] = estado.getConfigurado();
                res["habilitado"] = estado.getHabilitado();
                res["remitente"] = estado.getRemitente();
                return crow::response(200, res);
            } catch (const std::exception& e) {
                crow::json::wvalue error; error["detail"] = e.what(); return crow::response(500, error);
            }
        });

        CROW_ROUTE(app, "/api/verificar-correo").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
            try {
                auto body = crow::json::load(req.body);
                if (!body || !body.has("token")) {
                    crow::json::wvalue error; error["detail"] = "Falta el token de verificacion."; return crow::response(400, error);
                }

                DBConfig dbConfig;
                pqxx::connection conn(dbConfig.obtenerDatabaseUrl());
                pqxx::work txn(conn);
                const std::string token = body["token"].s();
                auto r = txn.exec("SELECT equipo_id, email, expira_en FROM email_verification_tokens WHERE token = $1 AND usado = false AND expira_en > NOW() LIMIT 1",
                                  pqxx::params{token});
                if (r.empty()) {
                    crow::json::wvalue res; res["detail"] = "El enlace no es válido o expiró."; return crow::response(410, res);
                }

                const auto row = r[0];
                const int equipoId = row["equipo_id"].as<int>();
                txn.exec("UPDATE email_verification_tokens SET usado = true WHERE token = $1", pqxx::params{token});
                txn.exec("UPDATE alumnos SET correo_verificado = true WHERE idequipo = $1", pqxx::params{equipoId});
                txn.commit();

                crow::json::wvalue res;
                res["correo"] = row["email"].as<std::string>();
                res["equipo"] = std::to_string(equipoId);
                return crow::response(200, res);
            } catch (const std::exception& e) {
                crow::json::wvalue error; error["detail"] = e.what(); return crow::response(500, error);
            }
        });

        CROW_ROUTE(app, "/api/verificar-correo/reenviar").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
            try {
                auto body = crow::json::load(req.body);
                if (!body || !body.has("correo")) {
                    crow::json::wvalue error; error["detail"] = "Falta el correo del líder."; return crow::response(400, error);
                }

                std::string correo = body["correo"].s();
                if (correo.empty()) {
                    return crow::response(202, crow::json::wvalue{{"status", "accepted"}});
                }

                DBConfig dbConfig;
                pqxx::connection conn(dbConfig.obtenerDatabaseUrl());
                pqxx::work txn(conn);
                const std::string correoSql = correo;
                auto r = txn.exec(
                    "SELECT e.idequipo, e.nombre_equipo "
                    "FROM equipo e "
                    "JOIN alumnos a ON a.idequipo = e.idequipo "
                    "WHERE LOWER(a.correo) = LOWER($1) "
                    "LIMIT 1",
                    pqxx::params{correoSql});
                if (r.empty()) {
                    txn.commit();
                    return crow::response(202, crow::json::wvalue{{"status", "accepted"}});
                }

                int equipoId = r[0]["idequipo"].as<int>();
                std::string token = "verify_" + std::to_string(equipoId) + "_" + std::to_string(std::time(nullptr));
                const std::string tokenSql = token;
                txn.exec("INSERT INTO email_verification_tokens (equipo_id, email, token, expira_en) VALUES ($1, $2, $3, NOW() + INTERVAL '24 hours') ON CONFLICT (token) DO NOTHING",
                         pqxx::params{equipoId, correoSql, tokenSql});
                txn.commit();
                std::string html = "<html><body><p>Hola,</p><p>Confirma tu correo para tu equipo.</p><p><a href=\"https://roadtotech.mx/registro/verificar?token=" + token + "\">Verificar correo</a></p></body></html>";
                EmailService::sendEmail(correo, "Verifica tu correo del equipo", html);
                return crow::response(202, crow::json::wvalue{{"status", "accepted"}});
            } catch (const std::exception& e) {
                crow::json::wvalue error; error["detail"] = e.what(); return crow::response(500, error);
            }
        });
    }
};
