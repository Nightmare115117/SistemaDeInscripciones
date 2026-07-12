#include "DBConfig.h"
#include <fstream>
#include <cstdlib>
#include <stdexcept>

DBConfig::DBConfig(const std::string& rutaEnv) {
    cargarEnv(rutaEnv);
    databaseUrl = leerDatabaseUrlDelEntorno();
}

void DBConfig::cargarEnv(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        return; // si no existe, puede que ya estén exportadas manualmente
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        auto pos = linea.find('=');
        if (pos == std::string::npos) continue;

        std::string clave = linea.substr(0, pos);
        std::string valor = linea.substr(pos + 1);

        setenv(clave.c_str(), valor.c_str(), 1);
    }
    archivo.close();
}

std::string DBConfig::leerDatabaseUrlDelEntorno() {
    const char* db_url = std::getenv("DATABASE_URL");
    if (!db_url) {
        throw std::runtime_error("DATABASE_URL no está definida ni en .env ni en el entorno.");
    }
    return std::string(db_url);
}

std::string DBConfig::obtenerDatabaseUrl() const {
    return databaseUrl;
}