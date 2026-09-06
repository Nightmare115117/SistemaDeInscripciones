#include "PaisRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace pqxx;

PaisRepository::PaisRepository(DBConfig& dbConfig) : dbConfig(dbConfig) {}

std::vector<PaisModel> PaisRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result rows = txn.exec("SELECT id_paises, nombre, codigo_iso2, codigo_iso3 FROM paises ORDER BY id_paises");

    std::vector<PaisModel> paises;
    for (const auto& row : rows) {
        PaisModel pais(row["nombre"].as<std::string>(), row["codigo_iso2"].as<std::string>(), row["codigo_iso3"].as<std::string>());
        pais.setId(row["id_paises"].as<int>());
        paises.push_back(pais);
    }
    return paises;
}

PaisModel PaisRepository::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result rows = txn.exec(
        "SELECT id_paises, nombre, codigo_iso2, codigo_iso3 FROM paises WHERE id_paises = $1", params{id});

    if (rows.empty()) throw std::logic_error("No existe un pais con el id mencionado");

    PaisModel pais(rows[0]["nombre"].as<std::string>(), rows[0]["codigo_iso2"].as<std::string>(), rows[0]["codigo_iso3"].as<std::string>());
    pais.setId(rows[0]["id_paises"].as<int>());
    return pais;
}

int PaisRepository::insert(const PaisModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result rows = txn.exec(
        "INSERT INTO paises (nombre, codigo_iso2, codigo_iso3) VALUES ($1, $2, $3) RETURNING id_paises",
        params{entity.getNombre(), entity.getCodigoIso2(), entity.getCodigoIso3()});
    txn.commit();
    return rows[0]["id_paises"].as<int>();
}

bool PaisRepository::update(const PaisModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result rows = txn.exec(R"sql(
        UPDATE paises SET
            nombre = CASE WHEN $1 <> '' THEN $1 ELSE nombre END,
            codigo_iso2 = CASE WHEN $2 <> '' THEN $2 ELSE codigo_iso2 END,
            codigo_iso3 = CASE WHEN $3 <> '' THEN $3 ELSE codigo_iso3 END
        WHERE id_paises = $4
    )sql",
        params{entity.getNombre(), entity.getCodigoIso2(), entity.getCodigoIso3(), entity.getId()});
    txn.commit();
    return rows.affected_rows() > 0;
}

bool PaisRepository::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result rows = txn.exec("DELETE FROM paises WHERE id_paises = $1", params{id});
    txn.commit();
    return rows.affected_rows() > 0;
}
