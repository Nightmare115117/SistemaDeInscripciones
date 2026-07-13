#include "RegistroRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

RegistroRepository::RegistroRepository(DBConfig& dbConfig) : dbConfig(dbConfig) {
}

vector<RegistroModel> RegistroRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idRegistro, Fecha_hora_registro ,idEquipo FROM Registro");

    vector<RegistroModel> lista;

    for (auto fila : r) {
        RegistroModel registro(fila ["idEquipo"].as<int>());
        registro.setId(fila ["idRegistro"].as<int>());
        registro.setFecha(fila ["Fecha_hora_registro"].as<string>());
        lista.push_back(registro);
    }

    return lista;
}

RegistroModel RegistroRepository::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idRegistro, Fecha_hora_registro, idEquipo FROM Registro WHERE idRegistro = $1", params{id});

    if (r.empty()){
        RegistroModel registro;
        registro.setId(-1);
        return registro;
    }

    RegistroModel registro(r[0]["idEquipo"].as<int>());
    registro.setId(r[0]["idRegistro"].as<int>());
    registro.setFecha(r[0]["Fecha_hora_registro"].as<string>());

    return registro;
}

int RegistroRepository::insert(const RegistroModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("INSERT INTO Registro (idEquipo) VALUES ($1) RETURNING idRegistro", params{
        entity.getIdEquipo() //parametro 1
    });

    txn.commit();
    return (r[0]["idRegistro"].as<int>());
}

bool RegistroRepository::update(const RegistroModel& entity) {
    throw logic_error("No se permite actualizar un registro una vez creado");
}

bool RegistroRepository::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("DELETE FROM Registro WHERE idREgistro = $1", 
        params{
            id //parametro 1
        });
    txn.commit();

    return r.affected_rows() > 0;
}