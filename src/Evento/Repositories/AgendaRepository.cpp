#include "AgendaRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace pqxx;
using namespace std;

AgendaModel AgendaRepository::hydrate(const string& id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    auto row = txn.exec("SELECT id, dia, inicio::text, fin::text, titulo, coalesce(detalle, ''), tipo FROM agenda WHERE id = $1", params{id}).one_row();
    AgendaModel item;
    item.setId(row[0].as<string>()); item.setDia(row[1].as<string>()); item.setInicio(row[2].as<string>()); item.setFin(row[3].as<string>());
    item.setTitulo(row[4].as<string>()); item.setDetalle(row[5].as<string>()); item.setTipo(row[6].as<string>());
    for (const auto& responsible : txn.exec("SELECT staff_id FROM agenda_responsable WHERE agenda_id = $1", params{id})) {
        auto responsables = item.getResponsables();
        responsables.push_back(responsible[0].as<string>());
        item.setResponsables(responsables);
    }
    return item;
}

vector<AgendaModel> AgendaRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    vector<AgendaModel> result;
    for (const auto& row : txn.exec("SELECT id FROM agenda ORDER BY dia, inicio"))
        result.push_back(hydrate(row[0].as<string>()));
    return result;
}

AgendaModel AgendaRepository::insert(const AgendaModel& item) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    auto row = txn.exec("INSERT INTO agenda (dia, inicio, fin, titulo, detalle, tipo) VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
        params{item.getDia(), item.getInicio(), item.getFin(), item.getTitulo(), item.getDetalle(), item.getTipo()}).one_row();
    string id = row[0].as<string>();
    for (const auto& staffId : item.getResponsables())
        txn.exec("INSERT INTO agenda_responsable (agenda_id, staff_id) VALUES ($1, $2)", params{id, staffId});
    txn.commit();
    return hydrate(id);
}

AgendaModel AgendaRepository::update(const string& id, const AgendaModel& item) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    txn.exec("UPDATE agenda SET dia = $1, inicio = $2, fin = $3, titulo = $4, detalle = $5, tipo = $6 WHERE id = $7",
        params{item.getDia(), item.getInicio(), item.getFin(), item.getTitulo(), item.getDetalle(), item.getTipo(), id});
    txn.exec("DELETE FROM agenda_responsable WHERE agenda_id = $1", params{id});
    for (const auto& staffId : item.getResponsables())
        txn.exec("INSERT INTO agenda_responsable (agenda_id, staff_id) VALUES ($1, $2)", params{id, staffId});
    txn.commit();
    return hydrate(id);
}

AgendaModel AgendaRepository::findById(const string& id) const { return hydrate(id); }
bool AgendaRepository::update(const AgendaModel& item) {
    update(item.getId(), item);
    return true;
}

bool AgendaRepository::remove(const string& id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    auto result = txn.exec("DELETE FROM agenda WHERE id = $1", params{id});
    txn.commit();
    return result.affected_rows() > 0;
}
