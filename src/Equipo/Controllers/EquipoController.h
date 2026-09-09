#pragma once
#include "Generics/Controller.h"
#include "Equipo/Models/EquipoModel.h"
#include "Equipo/Services/EquipoService.h"
#include "Equipo/Repositories/AlumnoRepository.h"
#include "Equipo/Repositories/ContactoEmergenciaRepository.h"
#include "Problematica/Repositories/ProblemaRepository.h"
#include "Email/EmailService.h"
#include "Security/Crypto.h"
#include <stdexcept>

class EquipoController : public Controller<EquipoModel, EquipoService> {
    AlumnoRepository& alumnoRepo;
    ContactoEmergenciaRepository& contactoRepo;
    ProblemaRepository& problemaRepo;

public:
    EquipoController(EquipoService& service, AlumnoRepository& alumnos,
        ContactoEmergenciaRepository& contactos, ProblemaRepository& problemas);

    crow::json::wvalue toJson(const EquipoModel& entidad) const override;
    EquipoModel fromJson(const crow::json::rvalue& json) const override;

    template <typename AppType>
    void rutasAdministrativas(AppType& app) {
        CROW_ROUTE(app, "/api/equipos/<int>").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req, int id) {
            try {
                auto body = crow::json::load(req.body);
                if (!body) throw std::invalid_argument("JSON invalido");
                std::string estado;
                std::string revisadoEn;
                std::string notas;
                if (body.has("estado")) estado = body["estado"].s();
                if (body.has("revisadoEn")) revisadoEn = body["revisadoEn"].s();
                if (body.has("notas")) notas = body["notas"].s();
                if (!service.updateReview(id, estado, revisadoEn, notas)) return crow::response(404);

                if (estado == "aceptado" || estado == "rechazado") {
                    try {
                        const auto equipo = service.findById(id);
                        if (equipo.getIdLider() > 0) {
                            const auto lider = alumnoRepo.findById(equipo.getIdLider());
                            const std::string emailDestino = AES::decrypt(lider.getCorreo());
                            std::string asunto = estado == "aceptado" ? "Tu equipo fue aceptado" : "Tu equipo fue revisado";
                            std::string cuerpo = "Hola,\n\nTu equipo \"" + equipo.getNombre() + "\" ha sido " + estado + ".\n\nGracias por participar.\n";
                            EmailService::sendEmail(emailDestino, asunto, cuerpo);
                        }
                    } catch (const std::exception&) {
                        // No falla la actualización si el correo no puede enviarse.
                    }
                }

                return crow::response(200, toJson(service.findById(id)));
            } catch (const std::exception& e) { crow::json::wvalue error; error["detail"] = e.what(); return crow::response(422, error); }
        });
    }
};
