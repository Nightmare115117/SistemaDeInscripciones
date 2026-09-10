#include "crow.h"
#include "crow/middlewares/cors.h"
#include "DBConfig/DBConfig.h"
#include "Middleware/AuthMiddleware.h"
#include "Middleware/RateLimitMiddleware.h"
#include <pqxx/pqxx>
#include <cstdlib>
#include <iostream>
#include "Email/EmailService.h"

#include "Admin/Repo/AdminRepo.h"
#include "Admin/Service/AdminService.h"
#include "Admin/Controller/AdminController.h"

#include "Equipo/Repositories/AlumnoRepository.h"
#include "Equipo/Services/AlumnoService.h"
#include "Equipo/Controllers/AlumnoController.h"

#include "Equipo/Repositories/ContactoEmergenciaRepository.h"
#include "Equipo/Services/ContactoEmergenciaService.h"
#include "Equipo/Controllers/ContactoEmergenciaController.h"

#include "Equipo/Repositories/EquipoRepository.h"
#include "Equipo/Services/EquipoService.h"
#include "Equipo/Controllers/EquipoController.h"

#include "Pais/Repositories/PaisRepository.h"
#include "Pais/Services/PaisService.h"
#include "Pais/Controllers/PaisController.h"

#include "NivelEstudio/Repositories/NivelEstudioRepository.h"
#include "NivelEstudio/Services/NivelEstudioService.h"
#include "NivelEstudio/Controllers/NivelEstudioController.h"

#include "Problematica/Repositories/ProblemaRepository.h"
#include "Problematica/Service/ProblemaService.h"
#include "Problematica/Controller/ProblemaController.h"

#include "Registro/Repositories/RegistroRepository.h"
#include "Registro/Services/RegistroService.h"
#include "Registro/Controllers/RegistroController.h"
#include "Security/TurnstileValidator.h"

#include "Evento/Repositories/EventoRepository.h"
#include "Evento/Repositories/AgendaRepository.h"
#include "Evento/Repositories/StaffRepository.h"
#include "Evento/Repositories/PatrocinadorRepository.h"
#include "Evento/Services/EventoService.h"
#include "Evento/Services/AgendaService.h"
#include "Evento/Services/StaffService.h"
#include "Evento/Services/PatrocinadorService.h"
#include "Evento/Controllers/EventoController.h"
#include "Evento/Controllers/AgendaController.h"
#include "Evento/Controllers/StaffController.h"
#include "Evento/Controllers/PatrocinadorController.h"

using namespace std;
using namespace pqxx;

using App = crow::App<crow::CORSHandler, RateLimitMiddleware, AuthMiddleware>;

/**  string obtenerDatabaseUrl() {
    const char* db_url = getenv("DATABASE_URL");
    if (!db_url) {
        throw runtime_error("DATABASE_URL no está definida. Exporta la variable de entorno.");
    }
    return string(db_url);
}
*/

int main() {
    App app;

    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors
        .global()
        .headers("Content-Type", "Authorization")
        .methods("GET"_method, "POST"_method, "PUT"_method, "PATCH"_method, "DELETE"_method)
        .origin("*");

    DBConfig config;
    string databaseUrl = config.obtenerDatabaseUrl();

    AlumnoRepository alumnoRepo(config);
    AlumnoService alumnoService(alumnoRepo);
    AlumnoController alumnoController(alumnoService);

    RegistroRepository registroRepo(config);

    EquipoRepository equipoRepo(config);
    EquipoService equipoService(equipoRepo);

    PaisRepository paisRepo(config);
    PaisService paisService(paisRepo);
    PaisController paisController(paisService);

    NivelEstudioRepository nivelEstudioRepo(config);
    NivelEstudioService nivelEstudioService(nivelEstudioRepo);
    NivelEstudioController nivelEstudioController(nivelEstudioService);

    ProblemaRepository problemaRepo(config);
    ProblemaService problemaService(problemaRepo);
    ProblemaController problemaController(problemaService);

    ContactoEmergenciaRepository contactoRepo(config);
    ContactoEmergenciaService contactoService(contactoRepo);
    ContactoEmergenciaController contactoController(contactoService);

    EquipoController equipoController(equipoService, alumnoRepo, contactoRepo, problemaRepo);

    EventoRepository eventoRepo(config);
    RegistroService registroService(registroRepo, equipoRepo, alumnoRepo, contactoRepo, eventoRepo);
    const char* turnstileSecret = getenv("TURNSTILE_SECRET_KEY");
    const char* turnstileHostname = getenv("TURNSTILE_EXPECTED_HOSTNAME");
    CurlTurnstileHttpClient turnstileHttpClient;
    TurnstileValidator turnstileValidator(
        turnstileSecret ? turnstileSecret : "",
        turnstileHostname ? turnstileHostname : "",
        turnstileHttpClient);
    RegistroController registroController(registroService, problemaRepo, turnstileValidator);

    EventoService eventoService(eventoRepo);
    EventoController eventoController(eventoService);
    AgendaRepository agendaRepo(config);
    AgendaService agendaService(agendaRepo);
    AgendaController agendaController(agendaService);
    StaffRepository staffRepo(config);
    StaffService staffService(staffRepo);
    StaffController staffController(staffService);
    PatrocinadorRepository patrocinadorRepo(config);
    PatrocinadorService patrocinadorService(patrocinadorRepo);
    PatrocinadorController patrocinadorController(patrocinadorService);

    PasswordHasher hasher = PasswordHasher(12);

    AdminRepo adminRepo(config);
    AdminService adminService(adminRepo, hasher);
    AdminController adminController(adminService);

    registroController.registrarRutas(app, "/api/registros");
    registroController.contadorDeRegistros(app, registroService);
    registroController.registroCompleto(app, registroService);

    equipoController.registrarRutas(app, "/api/equipos");
    equipoController.rutasAdministrativas(app);

    paisController.registrarRutas(app, "/api/paises");

    nivelEstudioController.registrarRutas(app, "/api/niveles-estudio");

    problemaController.registrarRutas(app, "/api/problematica");

    contactoController.registrarRutas(app, "/api/contactos-emergencia");

    adminController.HacerInicioDeSesion(app, adminService);

    adminController.quienSoy(app, adminService);

    alumnoController.registrarRutas(app, "/api/alumno");

    eventoController.registrarRutas(app);

    agendaController.registrarRutas(app);

    staffController.registrarRutas(app);
    
    patrocinadorController.registrarRutas(app);

    // Ruta de prueba de conexión
    CROW_ROUTE(app, "/api/health")
    ([databaseUrl](){
        try {
            pqxx::connection conn(databaseUrl);
            pqxx::work txn(conn);
            pqxx::result r = txn.exec("SELECT NOW()");
            txn.commit();

            crow::json::wvalue res;
            res["status"] = "ok";
            res["hora_servidor"] = r[0][0].c_str();
            return crow::response(200, res);
        } catch (const exception& e) {
            crow::json::wvalue res;
            res["status"] = "error";
            res["mensaje"] = e.what();
            return crow::response(500, res);
        }
    });

    CROW_ROUTE(app, "/test/email").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);
            if (!body) {
                crow::json::wvalue res;
                res["error"] = "JSON inválido";
                return crow::response(400, res);
            }

            string to = body["to"].s();
            string subject = body["subject"].s();
            string cuerpo = body["text"].s();

            bool result = EmailService::sendEmail(to, subject, cuerpo);

            if (result) {
               crow::json::wvalue res;
                res["Exito"] = "Correo enviado Exitosamente";
                return crow::response(200, res); 
            } else {
                crow::json::wvalue res;
                res["error"] = "Error inesperado, intente de nuevo";
                return crow::response(500, res);
            }
        } catch (const exception& e) {
            crow::json::wvalue res;
            res["error"] = "JSON inválido";
            return crow::response(500, res);
        }
    });

    app.port(8080).multithreaded().run();
}
