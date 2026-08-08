#include "crow.h"
#include "crow/middlewares/cors.h"
#include "DBConfig/DBConfig.h"
#include "Middleware/AuthMiddleware.h"
#include <pqxx/pqxx>
#include <cstdlib>
#include <iostream>

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

#include "Universidad/Repositories/UniversityRepository.h"
#include "Universidad/Services/UniversityService.h"
#include "Universidad/Controllers/UniversidadController.h"

#include "Problematica/Repositories/ProblemaRepository.h"
#include "Problematica/Service/ProblemaService.h"
#include "Problematica/Controller/ProblemaController.h"

#include "Registro/Repositories/RegistroRepository.h"
#include "Registro/Services/RegistroService.h"
#include "Registro/Controllers/RegistroController.h"

using namespace std;
using namespace pqxx;

using App = crow::App<crow::CORSHandler, AuthMiddleware>;

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
        .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method)
        .origin("*");

    DBConfig config;
    string databaseUrl = config.obtenerDatabaseUrl();

    AlumnoRepository alumnoRepo(config);
    AlumnoService alumnoService(alumnoRepo);
    AlumnoController alumnoController(alumnoService);

    RegistroRepository registroRepo(config);
    RegistroService registroService(registroRepo);
    RegistroController registroController(registroService);

    EquipoRepository equipoRepo(config);
    EquipoService equipoService(equipoRepo);
    EquipoController equipoController(equipoService);

    UniversityRepository universityRepo(config);
    UniversityService universityService(universityRepo);
    UniversidadController universidadController(universityService);

    ProblemaRepository problemaRepo(config);
    ProblemaService problemaService(problemaRepo);
    ProblemaController problemaController(problemaService);

    ContactoEmergenciaRepository contactoRepo(config);
    ContactoEmergenciaService contactoService(contactoRepo);
    ContactoEmergenciaController contactoController(contactoService);

    PasswordHasher hasher = PasswordHasher(12);

    AdminRepo adminRepo(config);
    AdminService adminService(adminRepo, hasher);
    AdminController adminController(adminService);

    registroController.registrarRutas(app, "/api/registros");
    registroController.contadorDeRegistros(app, registroService);
    equipoController.registrarRutas(app, "/api/equipos");
    universidadController.registrarRutas(app, "/api/universidades");
    problemaController.registrarRutas(app, "/api/problematica");
    contactoController.registrarRutas(app, "/api/contactos-emergencia");
    adminController.registrarRutas(app, "/api/admin");
    adminController.HacerInicioDeSesion(app, adminService);
    alumnoController.registrarRutas(app, "/api/alumno");

    // Ruta de prueba de conexión
    CROW_ROUTE(app, "/api/test-db")
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

    app.port(8080).multithreaded().run();
}