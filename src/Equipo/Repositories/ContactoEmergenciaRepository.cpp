#include "ContactoEmergenciaRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

ContactoEmergenciaRepository::ContactoEmergenciaRepository(DBConfig& dbConfig)
    : dbConfig(dbConfig) {}

vector<ContactoEmergenciaModel> ContactoEmergenciaRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idcontacto, nombre, telefono, pariente FROM contactoemergencia");

    vector<ContactoEmergenciaModel> lista;
    for (const auto &fila : r) {
        ContactoEmergenciaModel contacto(
            fila["nombre"].as<string>(),
            fila["telefono"].as<string>(),
            fila["pariente"].as<string>()
        );
        contacto.setId(fila["idcontacto"].as<int>());
        lista.push_back(contacto);
    }

    return lista;
}

ContactoEmergenciaModel ContactoEmergenciaRepository::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec("SELECT idcontacto, nombre, telefono, pariente FROM contactoemergencia WHERE idcontacto = $1", params{id});

    if (r.empty()) {
        throw logic_error("No existe un contacto de emergencia con el id mencionado");
    }

    ContactoEmergenciaModel contacto(
        r[0]["nombre"].as<string>(),
        r[0]["telefono"].as<string>(),
        r[0]["pariente"].as<string>()
    );
    contacto.setId(r[0]["idcontacto"].as<int>());
    return contacto;
}

int ContactoEmergenciaRepository::insert(const ContactoEmergenciaModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec(R"sql(INSERT INTO contactoemergencia (nombre, telefono, pariente)
        VALUES ($1, $2, $3)
        RETURNING idcontacto)sql", params{
        entity.getNombre(),
        entity.getTelefono(),
        entity.getPariente()
    });

    txn.commit();
    return r[0]["idcontacto"].as<int>();
}

bool ContactoEmergenciaRepository::update(const ContactoEmergenciaModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec(R"sql(UPDATE contactoemergencia SET
        nombre = CASE WHEN $1 <> '' THEN $1 ELSE nombre END,
        telefono = CASE WHEN $2 <> '' THEN $2 ELSE telefono END,
        pariente = CASE WHEN $3 <> '' THEN $3 ELSE pariente END
        WHERE idcontacto = $4)sql", params{
        entity.getNombre(),
        entity.getTelefono(),
        entity.getPariente(),
        entity.getId()
    });

    txn.commit();
    return r.affected_rows() > 0;
}

bool ContactoEmergenciaRepository::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("DELETE FROM contactoemergencia WHERE idcontacto = $1", params{id});
    txn.commit();
    return r.affected_rows() > 0;
}
