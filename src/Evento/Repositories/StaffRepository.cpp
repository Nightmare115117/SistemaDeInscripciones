#include "StaffRepository.h"
#include <pqxx/pqxx>
using namespace pqxx;
using namespace std;

static StaffModel staffFromRow(const auto& row) {
    StaffModel item;
    item.setId(row[0].template as<string>()); item.setNombre(row[1].template as<string>());
    item.setRol(row[2].template as<string>()); item.setArea(row[3].template as<string>());
    item.setCorreo(row[4].template as<string>()); item.setTelefono(row[5].template as<string>());
    return item;
}

vector<StaffModel> StaffRepository::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    vector<StaffModel> result;
    for (const auto& row : txn.exec("SELECT id, nombre, rol, area, correo, telefono FROM staff ORDER BY nombre"))
        result.push_back(staffFromRow(row));
    return result;
}

StaffModel StaffRepository::insert(const StaffModel& item) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    auto row = txn.exec("INSERT INTO staff (nombre, rol, area, correo, telefono) VALUES ($1, $2, $3, $4, $5) RETURNING id, nombre, rol, area, correo, telefono", params{item.getNombre(), item.getRol(), item.getArea(), item.getCorreo(), item.getTelefono()}).one_row();
    txn.commit();
    return staffFromRow(row);
}

StaffModel StaffRepository::update(const string& id, const StaffModel& item) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    auto row = txn.exec("UPDATE staff SET nombre = coalesce(nullif($1, ''), nombre), rol = coalesce(nullif($2, ''), rol), area = coalesce(nullif($3, ''), area), correo = coalesce(nullif($4, ''), correo), telefono = coalesce(nullif($5, ''), telefono) WHERE id = $6 RETURNING id, nombre, rol, area, correo, telefono", params{item.getNombre(), item.getRol(), item.getArea(), item.getCorreo(), item.getTelefono(), id}).one_row();
    txn.commit();
    return staffFromRow(row);
}

bool StaffRepository::remove(const string& id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    auto result = txn.exec("DELETE FROM staff WHERE id = $1", params{id});
    txn.commit();
    return result.affected_rows() > 0;
}
