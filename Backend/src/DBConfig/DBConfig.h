#pragma once
#include <string>

class DBConfig {
public:
    explicit DBConfig(const std::string& rutaEnv = ".env");

    std::string obtenerDatabaseUrl() const;

private:
    std::string databaseUrl;
    void cargarEnv(const std::string& ruta);
    std::string leerDatabaseUrlDelEntorno();
};