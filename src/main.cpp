#include "crow.h"
#include "DBConfig/DBConfig.h"
#include <pqxx/pqxx>
#include <cstdlib>
#include <iostream>

#include "Registro/Repositories/RegistroRepository.h"
#include "Registro/Services/RegistroService.h"
#include "Registro/Controllers/RegistroController.h"

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
    RegistroController RegistroController(registroService);

    RegistroController.registrarRutas(app, "/api/registros");

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