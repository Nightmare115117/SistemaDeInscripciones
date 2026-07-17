#include "RegistroRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace pqxx;

RegistroRepository::RegistroRepository(DBConfig& dbConfig) : dbConfig(dbConfig) {
}

vector<RegistroModel> RegistroRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idregistro, fecha_hora_registro, idequipo FROM registro");

    vector<RegistroModel> lista;

    for (const auto &fila : r) {
        RegistroModel registro(fila["idequipo"].as<int>());
        registro.setId(fila["idregistro"].as<int>());
        registro.setFecha(fila["fecha_hora_registro"].as<string>());
        lista.push_back(registro);
    }

    return lista;
}

RegistroModel RegistroRepository::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idregistro, fecha_hora_registro, idequipo FROM registro WHERE idregistro = $1", params{id});

    if (r.empty()) {
        RegistroModel registro;
        registro.setId(-1);
        return registro;
    }

    RegistroModel registro(r[0]["idequipo"].as<int>());
    registro.setId(r[0]["idregistro"].as<int>());
    registro.setFecha(r[0]["fecha_hora_registro"].as<string>());

    return registro;
}

int RegistroRepository::insert(const RegistroModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("INSERT INTO registro (idequipo) VALUES ($1) RETURNING idregistro", params{
        entity.getIdEquipo() // parametro 1
    });

    txn.commit();
    return (r[0]["idregistro"].as<int>());
}

bool RegistroRepository::update(const RegistroModel& entity) {
    throw logic_error("No se permite actualizar un registro una vez creado");
}

bool RegistroRepository::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("DELETE FROM registro WHERE idregistro = $1",
        params{
            id // parametro 1
        });
    txn.commit();

    return r.affected_rows() > 0;
}

int RegistroRepository::countById() const {
    try {
        connection conn(dbConfig.obtenerDatabaseUrl());
        nontransaction txn(conn);
        result r = txn.exec("SELECT COUNT(idregistro) FROM registro");
        return r[0][0].as<int>();
    } catch (const exception& e) {
        cerr << "Error al contar registros: " << e.what() << endl;
    throw;
    }
}