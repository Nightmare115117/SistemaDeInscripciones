#pragma once
#include "Generics/Controller.h"
#include "Registro/Models/RegistroModel.h"
#include "Registro/Services/RegistroService.h"
#include "Problematica/Repositories/ProblemaRepository.h"
#include <stdexcept>
#include <string>
#include <vector>

class RegistroController : public Controller <RegistroModel, RegistroService> {
    ProblemaRepository& problemaRepo;

public:
    RegistroController(RegistroService& service, ProblemaRepository& problemRepository);

    crow::json::wvalue toJson(const RegistroModel& entidad) const override;
    RegistroModel fromJson(const crow::json::rvalue& json) const override;

    template <typename AppType>
    void contadorDeRegistros(AppType& app, const RegistroService& service) {
        CROW_ROUTE(app, "/api/registro/count")([&service]() {
            try {
                crow::json::wvalue res;
                auto count = service.countStats();
                res["total"] = count.total;
                res["aceptados"] = count.aceptados;
                res["pendientes"] = count.pendientes;
                res["cupo"] = count.cupo;
                res["disponibles"] = count.disponibles;
                res["abierto"] = count.abierto;
                return crow::response(200, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["Error"] = e.what();
                return crow::response(500, res);
            }
        });
    }

    template <typename AppType>
    void registroCompleto(AppType& app, RegistroService& service) {
        CROW_ROUTE(app, "/api/registro")
        .methods(crow::HTTPMethod::POST)([this, &service](const crow::request& req) {
            try {
                auto body = crow::json::load(req.body);
                if (!body || !body.has("equipo") || !body.has("problematica") ||
                    !body.has("integrantes") || !body.has("aceptaReglamento")) {
                    crow::json::wvalue error;
                    error["detail"] = "Faltan campos obligatorios del registro";
                    return crow::response(422, error);
                }

                std::vector<AlumnoModel> alumnos;
                std::vector<ContactoEmergenciaModel> contactos;
                for (const auto& integrante : body["integrantes"]) {
                    if (!integrante.has("nombre") || !integrante.has("correo") ||
                        !integrante.has("telefono") || !integrante.has("institucion") ||
                        !integrante.has("emergencia")) {
                        throw std::invalid_argument("Cada integrante requiere sus datos y una emergencia");
                    }

                    const auto& emergencia = integrante["emergencia"];
                    if (!emergencia.has("nombre") || !emergencia.has("telefono") ||
                        !emergencia.has("parentesco")) {
                        throw std::invalid_argument("El contacto de emergencia está incompleto");
                    }

                    AlumnoModel alumno;
                    alumno.setNombre(integrante["nombre"].s());
                    alumno.setCorreo(integrante["correo"].s());
                    alumno.setNumeroTel(integrante["telefono"].s());
                    std::string alergias;
                    if (integrante.has("alergias")) {
                        alergias = integrante["alergias"].s();
                    }
                    alumno.setAlergias(alergias);
                    alumno.setFirmoTerminos(body["aceptaReglamento"].b());
                    alumno.setIdEquipo(-1);
                    alumno.setIdContacto(-1);
                    alumno.setIdUniversidad(-1);
                    alumnos.push_back(alumno);

                    contactos.emplace_back(
                        emergencia["nombre"].s(),
                        emergencia["telefono"].s(),
                        emergencia["parentesco"].s());
                }

                if (alumnos.empty()) {
                    throw std::invalid_argument("El equipo debe tener al menos un integrante");
                }

                int idProblematica = problemaRepo.findIdByCodigo(body["problematica"].s());
                EquipoModel equipo(body["equipo"].s(), -1, -1, idProblematica);
                RegistroModel registro(-1);
                int id = service.insertRegistroCompleto(registro, equipo, alumnos, contactos);
                crow::json::wvalue res;
                res["id"] = id;
                res["nombre"] = body["equipo"].s();
                res["estado"] = "pendiente";
                res["mensaje"] = "Solicitud recibida. El comité organizador revisa cada registro y confirma por correo al líder del equipo.";
                return crow::response(201, res);
            } catch (const std::exception& e) {
                crow::json::wvalue res;
                res["detail"] = e.what();
                return crow::response(422, res);
            }
        });
    }
};