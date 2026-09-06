#include "NivelEstudioRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace pqxx;

NivelEstudioRepository::NivelEstudioRepository(DBConfig& dbConfig) : dbConfig(dbConfig) {}

std::vector<NivelEstudioModel> NivelEstudioRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result rows = txn.exec("SELECT id, nivel FROM nivel_estudio ORDER BY id");

    std::vector<NivelEstudioModel> niveles;
    for (const auto& row : rows) {
        NivelEstudioModel nivel(row["nivel"].as<std::string>());
        nivel.setId(row["id"].as<int>());
        niveles.push_back(nivel);
    }
    return niveles;
}

NivelEstudioModel NivelEstudioRepository::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result rows = txn.exec(
        "SELECT id, nivel FROM nivel_estudio WHERE id = $1", params{id});

    if (rows.empty()) throw std::logic_error("No existe un nivel de estudio con el id mencionado");

    NivelEstudioModel nivel(rows[0]["nivel"].as<std::string>());
    nivel.setId(rows[0]["id"].as<int>());
    return nivel;
}

int NivelEstudioRepository::insert(const NivelEstudioModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result rows = txn.exec(
        "INSERT INTO nivel_estudio (nivel) VALUES ($1) RETURNING id",
        params{entity.getNivel()});
    txn.commit();
    return rows[0]["id"].as<int>();
}

bool NivelEstudioRepository::update(const NivelEstudioModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result rows = txn.exec(
        "UPDATE nivel_estudio SET nivel = CASE WHEN $1 <> '' THEN $1 ELSE nivel END WHERE id = $2",
        params{entity.getNivel(), entity.getId()});
    txn.commit();
    return rows.affected_rows() > 0;
}

bool NivelEstudioRepository::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result rows = txn.exec("DELETE FROM nivel_estudio WHERE id = $1", params{id});
    txn.commit();
    return rows.affected_rows() > 0;
}
