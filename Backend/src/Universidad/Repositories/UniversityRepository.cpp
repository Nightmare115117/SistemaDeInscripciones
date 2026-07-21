#include "UniversityRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

UniversityRepository::UniversityRepository(DBConfig& dbConfig)
    : dbConfig(dbConfig) {}

vector<UniversityModel> UniversityRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT iduniversidad, nombre_uni FROM universidades");

    vector<UniversityModel> lista;
    for (const auto &fila : r) {
        UniversityModel universidad(fila["nombre_uni"].as<string>());
        universidad.setId(fila["iduniversidad"].as<int>());
        lista.push_back(universidad);
    }

    return lista;
}

UniversityModel UniversityRepository::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT iduniversidad, nombre_uni FROM universidades WHERE iduniversidad = $1", params{id});

    if (r.empty()) {
        throw logic_error("No existe una universidad con el id mencionado");
    }

    UniversityModel universidad(r[0]["nombre_uni"].as<string>());
    universidad.setId(r[0]["iduniversidad"].as<int>());
    return universidad;
}

int UniversityRepository::insert(const UniversityModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("INSERT INTO universidades (nombre_uni) VALUES ($1) RETURNING iduniversidad", params{
        entity.getNombre()
    });

    txn.commit();
    return r[0]["iduniversidad"].as<int>();
}

bool UniversityRepository::update(const UniversityModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("UPDATE universidades SET nombre_uni = CASE WHEN $1 <> '' THEN $1 ELSE nombre_uni END WHERE iduniversidad = $2", params{
        entity.getNombre(),
        entity.getId()
    });

    txn.commit();
    return r.affected_rows() > 0;
}

bool UniversityRepository::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("DELETE FROM universidades WHERE iduniversidad = $1", params{id});
    txn.commit();
    return r.affected_rows() > 0;
}
