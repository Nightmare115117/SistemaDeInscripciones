#pragma once
#include "crow.h"
#include <string>

template <typename T, typename Service>
class Controller {

protected:
    Service& service;

public:
    explicit Controller(Service& service) : service(service) {}
    virtual ~Controller() = default;

    // Cada Controller específico debe saber convertir su Model a JSON y viceversa
    virtual crow::json::wvalue toJson(const T& entidad) const = 0;
    virtual T fromJson(const crow::json::rvalue& json) const = 0;

    void registrarRutas(crow::SimpleApp& app, const std::string& basePath) {

        // GET /basePath  -> listar todos
        app.route_dynamic(basePath)
        .methods(crow::HTTPMethod::GET)
        ([this](){
            try {
                auto lista = service.findAll();
                std::vector<crow::json::wvalue> jsonLista;
                for (const auto& item : lista) {
                    jsonLista.push_back(toJson(item));
                }
                crow::json::wvalue res;
                res = std::move(jsonLista);
                return crow::response(200, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["error"] = e.what();
                return crow::response(500, res);
            }
        });

        // GET /basePath/<id> -> obtener por id
        app.route_dynamic(basePath + "/<int>")
        .methods(crow::HTTPMethod::GET)
        ([this](int id){
            try {
                auto entidad = service.findById(id);
                return crow::response(200, toJson(entidad));
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["error"] = e.what();
                return crow::response(404, res);
            }
        });

        // POST /basePath -> insertar
        app.route_dynamic(basePath + "/insert")
        .methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req){
            try {
                auto body = crow::json::load(req.body);
                if (!body) {
                    crow::json::wvalue res;
                    res["error"] = "JSON inválido";
                    return crow::response(400, res);
                }
                T entidad = fromJson(body);
                int id = service.insert(entidad);
                crow::json::wvalue res;
                res["id"] = id;
                return crow::response(201, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["error"] = e.what();
                return crow::response(400, res);
            }
        });

        // PUT /basePath/<id> -> actualizar
        app.route_dynamic(basePath + "/update/<int>")
        .methods(crow::HTTPMethod::PUT)
        ([this](const crow::request& req, int id){
            try {
                auto body = crow::json::load(req.body);
                if (!body) {
                    crow::json::wvalue res;
                    res["error"] = "JSON inválido";
                    return crow::response(400, res);
                }
                T entidad = fromJson(body);
                entidad.setId(id);
                bool actualizado = service.update(entidad);
                crow::json::wvalue res;
                res["actualizado"] = actualizado;
                return crow::response(200, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["error"] = e.what();
                return crow::response(400, res);
            }
        });

        // DELETE /basePath/remove/<id> -> eliminar
        app.route_dynamic(basePath + "/remove/<int>")
        .methods(crow::HTTPMethod::DELETE)
        ([this](int id){
            try {
                bool eliminado = service.remove(id);
                crow::json::wvalue res;
                res["eliminado"] = eliminado;
                return crow::response(200, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["error"] = e.what();
                return crow::response(400, res);
            }
        });
    }
};