#include "PatrocinadorRepository.h"
#include <pqxx/pqxx>
using namespace pqxx;
using namespace std;

static PatrocinadorModel sponsorFromRow(const auto& row) {
    PatrocinadorModel item;
    item.setId(row[0].template as<string>()); item.setNombre(row[1].template as<string>());
    item.setLogo(row[2].template as<string>()); item.setUrl(row[3].template as<string>());
    item.setNivel(row[4].template as<string>());
    return item;
}

vector<PatrocinadorModel> PatrocinadorRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    vector<PatrocinadorModel> result;
    for (const auto& row : txn.exec("SELECT id, nombre, logo, url, nivel FROM patrocinadores ORDER BY nombre"))
        result.push_back(sponsorFromRow(row));
    return result;
}

PatrocinadorModel PatrocinadorRepository::insert(const PatrocinadorModel& item) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    auto row = txn.exec("INSERT INTO patrocinadores (nombre, logo, url, nivel) VALUES ($1, $2, $3, $4) RETURNING id, nombre, logo, url, nivel", params{item.getNombre(), item.getLogo(), item.getUrl(), item.getNivel()}).one_row();
    txn.commit();
    return sponsorFromRow(row);
}

PatrocinadorModel PatrocinadorRepository::findById(const string& id) const {
    connection conn(dbConfig.obtenerDatabaseUrl()); nontransaction txn(conn);
    return sponsorFromRow(txn.exec("SELECT id, nombre, logo, url, nivel FROM patrocinadores WHERE id = $1", params{id}).one_row());
}
bool PatrocinadorRepository::update(const PatrocinadorModel& item) {
    update(item.getId(), item);
    return true;
}

PatrocinadorModel PatrocinadorRepository::update(const string& id, const PatrocinadorModel& item) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    auto row = txn.exec("UPDATE patrocinadores SET nombre = coalesce(nullif($1, ''), nombre), logo = coalesce(nullif($2, ''), logo), url = coalesce(nullif($3, ''), url), nivel = coalesce(nullif($4, ''), nivel) WHERE id = $5 RETURNING id, nombre, logo, url, nivel", params{item.getNombre(), item.getLogo(), item.getUrl(), item.getNivel(), id}).one_row();
    txn.commit();
    return sponsorFromRow(row);
}

bool PatrocinadorRepository::remove(const string& id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    auto result = txn.exec("DELETE FROM patrocinadores WHERE id = $1", params{id});
    txn.commit();
    return result.affected_rows() > 0;
}
