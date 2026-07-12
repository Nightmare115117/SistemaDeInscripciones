#include "RegistroRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

RegistroRepository::RegistroRepository(DBConfig& dbConfig) : dbConfig(dbConfig) {
}

vector<RegistroModel> RegistroRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec( "SELECT idRegistro, Fecha_hota_registro ,idEquipo FROM Registro");
    txn.commit();

    vector<RegistroModel> lista;

    for (auto fila : r) {
        RegistroModel registro(fila ["idEquipo"].as<int>());
        registro.setId(fila ["idRegistro"].as<int>());
        registro.setFecha(fila ["Fecha_hora_registro"].as<string>());
    }

    return lista;
}

RegistroModel RegistroRepository::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
}