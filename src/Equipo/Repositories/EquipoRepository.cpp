#include "EquipoRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

EquipoRepository::EquipoRepository(DBConfig& dbConfig) : dbConfig(dbConfig) {}

vector<EquipoModel> EquipoRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idequipo, nombre_equipo, iduniversidad, idlider, idproblematica, estado, registrado_en::text, coalesce(revisado_en::text, '') AS revisado_en_text, coalesce(notas, '') AS notas_text FROM equipo");

    vector<EquipoModel> lista;
    for (const auto &fila : r) {
        EquipoModel equipo(
            fila["nombre_equipo"].as<string>(),
            fila["iduniversidad"].is_null() ? -1 : fila["iduniversidad"].as<int>(),
            fila["idlider"].is_null() ? -1 : fila["idlider"].as<int>(),
            fila["idproblematica"].as<int>()
        );
        equipo.setId(fila["idequipo"].as<int>());
        equipo.setEstado(fila["estado"].as<string>()); equipo.setRegistradoEn(fila["registrado_en"].as<string>());
        equipo.setRevisadoEn(fila["revisado_en_text"].as<string>()); equipo.setNotas(fila["notas_text"].as<string>());
        lista.push_back(equipo);
    }

    return lista;
}

EquipoModel EquipoRepository::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idequipo, nombre_equipo, iduniversidad, idlider, idproblematica, estado, registrado_en::text, coalesce(revisado_en::text, '') AS revisado_en_text, coalesce(notas, '') AS notas_text FROM equipo WHERE idequipo = $1", params{id});

    if (r.empty()) {
        throw logic_error("No existe un equipo con el id mencionado");
    }

    EquipoModel equipo(
        r[0]["nombre_equipo"].as<string>(),
        r[0]["iduniversidad"].is_null() ? -1 : r[0]["iduniversidad"].as<int>(),
        r[0]["idlider"].is_null() ? -1 : r[0]["idlider"].as<int>(),
        r[0]["idproblematica"].as<int>()
    );
    equipo.setId(r[0]["idequipo"].as<int>());
    equipo.setEstado(r[0]["estado"].as<string>()); equipo.setRegistradoEn(r[0]["registrado_en"].as<string>());
    equipo.setRevisadoEn(r[0]["revisado_en_text"].as<string>()); equipo.setNotas(r[0]["notas_text"].as<string>());
    return equipo;
}

int EquipoRepository::insert(const EquipoModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec(R"sql(INSERT INTO equipo (nombre_equipo, iduniversidad, idlider, idproblematica)
        VALUES ($1, $2, $3, $4)
        RETURNING idequipo)sql", params{
        entity.getNombre(),
        entity.getIdUniversidad(),
        entity.getIdLider(),
        entity.getIdProblematica()
    });

    txn.commit();
    return r[0]["idequipo"].as<int>();
}

bool EquipoRepository::update(const EquipoModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec(R"sql(UPDATE equipo SET
        nombre_equipo = CASE WHEN $1 <> '' THEN $1 ELSE nombre_equipo END,
        iduniversidad = CASE WHEN $2 <> -1 THEN $2 ELSE iduniversidad END,
        idlider = CASE WHEN $3 <> -1 THEN $3 ELSE idlider END,
        idproblematica = CASE WHEN $4 <> -1 THEN $4 ELSE idproblematica END
        WHERE idequipo = $5)sql", params{
        entity.getNombre(),
        entity.getIdLider(),
        entity.getIdProblematica(),
        entity.getId()
    });

    txn.commit();
    return r.affected_rows() > 0;
}

bool EquipoRepository::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("DELETE FROM equipo WHERE idequipo = $1", params{id});
    txn.commit();
    return r.affected_rows() > 0;
}

bool EquipoRepository::updateReview(int id, const string& estado, const string& revisadoEn, const string& notas) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("UPDATE equipo SET estado = coalesce(nullif($1, ''), estado), revisado_en = nullif($2, ''), notas = CASE WHEN $3 <> '' THEN $3 ELSE notas END WHERE idequipo = $4", params{estado, revisadoEn, notas, id});
    txn.commit();
    return r.affected_rows() > 0;
}
