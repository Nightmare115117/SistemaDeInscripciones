#include "ProblemaRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

ProblemaRepository::ProblemaRepository(DBConfig& dbConfig)
    : dbConfig(dbConfig) {}

vector<ProblemaModel> ProblemaRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idproblematica, nombre, descripcion, coalesce(codigo, '') AS codigo FROM problematica");

    vector<ProblemaModel> lista;
    for (const auto &fila : r) {
        ProblemaModel problema(fila["nombre"].as<string>(), fila["descripcion"].as<string>(), fila["codigo"].as<string>());
        problema.setId(fila["idproblematica"].as<int>());
        lista.push_back(problema);
    }

    return lista;
}

ProblemaModel ProblemaRepository::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idproblematica, nombre, descripcion, coalesce(codigo, '') AS codigo FROM problematica WHERE idproblematica = $1", params{id});

    if (r.empty()) {
        throw logic_error("No existe una problematica con el id mencionado");
    }

    ProblemaModel problema(r[0]["nombre"].as<string>(), r[0]["descripcion"].as<string>(), r[0]["codigo"].as<string>());
    problema.setId(r[0]["idproblematica"].as<int>());
    return problema;
}

string ProblemaRepository::findCodigoById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT coalesce(codigo, idproblematica::text) AS codigo FROM problematica WHERE idproblematica = $1", params{id});
    if (r.empty()) throw logic_error("No existe una problematica con el id mencionado");
    return r[0]["codigo"].as<string>();
}

int ProblemaRepository::findIdByCodigo(const string& codigo) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idproblematica FROM problematica WHERE codigo = $1 OR idproblematica::text = $1 LIMIT 1", params{codigo});
    if (r.empty()) throw logic_error("La problematica no existe");
    return r[0]["idproblematica"].as<int>();
}

int ProblemaRepository::insert(const ProblemaModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec(R"sql(INSERT INTO problematica (nombre, descripcion, codigo)
        VALUES ($1, $2, $3)
        RETURNING idproblematica)sql", params{
        entity.getNombre(),
        entity.getDescripcion(),
        entity.getCodigo()
    });

    txn.commit();
    return r[0]["idproblematica"].as<int>();
}

bool ProblemaRepository::update(const ProblemaModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec(R"sql(UPDATE problematica SET
        nombre = CASE WHEN $1 <> '' THEN $1 ELSE nombre END,
        descripcion = CASE WHEN $2 <> '' THEN $2 ELSE descripcion END,
        codigo = CASE WHEN $3 <> '' THEN $3 ELSE codigo END
        WHERE idproblematica = $4)sql", params{
        entity.getNombre(),
        entity.getDescripcion(),
        entity.getCodigo(),
        entity.getId()
    });

    txn.commit();
    return r.affected_rows() > 0;
}

bool ProblemaRepository::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("DELETE FROM problematica WHERE idproblematica = $1", params{id});
    txn.commit();
    return r.affected_rows() > 0;
}
