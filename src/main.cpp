#include "crow.h"
#include "DBConfig/DBConfig.h"
#include <pqxx/pqxx>
#include <cstdlib>
#include <iostream>

#include "Registro/Repositories/RegistroRepository.h"
#include "Registro/Services/RegistroService.h"
#include "Registro/Controllers/RegistroController.h"
#include "Equipo/Repositories/EquipoRepository.h"
#include "Equipo/Services/EquipoService.h"
#include "Equipo/Controllers/EquipoController.h"
#include "Universidad/Repositories/UniversityRepository.h"
#include "Universidad/Services/UniversityService.h"
#include "Universidad/Controllers/UniversidadController.h"
#include "Problematica/Repositoryie/ProblemaRepository.h"
#include "Problematica/Service/ProblemaService.h"
#include "Problematica/Controller/ProblemaController.h"
#include "Equipo/Repositories/ContactoEmergenciaRepository.h"
#include "Equipo/Services/ContactoEmergenciaService.h"
#include "Equipo/Controllers/ContactoEmergenciaController.h"

using namespace std;
using namespace crow;
using namespace pqxx;

/**  string obtenerDatabaseUrl() {
    const char* db_url = getenv("DATABASE_URL");
    if (!db_url) {
        throw runtime_error("DATABASE_URL no está definida. Exporta la variable de entorno.");
    }
    return string(db_url);
}
*/

int main() {
    SimpleApp app;
    DBConfig config;
    string databaseUrl = config.obtenerDatabaseUrl();

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

    registroController.registrarRutas(app, "/api/registros");
    equipoController.registrarRutas(app, "/api/equipos");
    universidadController.registrarRutas(app, "/api/universidades");
    problemaController.registrarRutas(app, "/api/problematica");
    contactoController.registrarRutas(app, "/api/contactos-emergencia");

    // Ruta de prueba de conexión
    CROW_ROUTE(app, "/api/test-db")
    ([databaseUrl](){
        try {
            pqxx::connection conn(databaseUrl);
            pqxx::work txn(conn);
            pqxx::result r = txn.exec("SELECT NOW()");
            txn.commit();

            json::wvalue res;
            res["status"] = "ok";
            res["hora_servidor"] = r[0][0].c_str();
            return response(200, res);
        } catch (const exception& e) {
            json::wvalue res;
            res["status"] = "error";
            res["mensaje"] = e.what();
            return response(500, res);
        }
    });

    app.port(8080).multithreaded().run();
}