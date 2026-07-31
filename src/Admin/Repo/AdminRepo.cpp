#include "AdminRepo.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

AdminRepo::AdminRepo(DBConfig& dbConfig) : dbConfig(dbConfig) {}

vector<AdminModel> AdminRepo::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT adminid, nombre, correo, contrasena FROM admin)sql");

    vector<AdminModel> lista;

    for (const auto& fila : r) {
        AdminModel admin(fila["nombre"].as<string>());
            admin.setId(fila["adminid"].as<int>());
            admin.setCorreo(fila["correo"].as<string>());
            admin.setContrasena(fila["contrasena"].as<string>());
        lista.push_back(admin);
    }

    return lista;
}

AdminModel AdminRepo::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT adminid, 
        nombre, correo, 
        contrasena FROM admin WHERE adminid = $1)sql",
        params{
            id // param $1
        });
    
    if (r.empty()) 
        throw logic_error("No existe un Admin con el id mencionado");

    AdminModel admin(r[0]["nombre"].as<string>());
        admin.setId(r[0]["adminid"].as<int>());
        admin.setCorreo(r[0]["correo"].as<string>());
        admin.setContrasena(r[0]["contrasena"].as<string>());
    
    return admin;
}

int AdminRepo::insert(const AdminModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec(R"sql(INSERT INTO admin 
        (nombre, 
        correo,
        contrasena) 
        VALUES ($1, $2, $3) 
        RETURNING adminid)sql", params{
        entity.getNombre(),    //param 1
        entity.getCorreo(),    //param 2
        entity.getContrasena() //param 3
    });

    txn.commit();
    return r[0]["adminid"].as<int>();
}

bool AdminRepo::update(const AdminModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);

    result r = txn.exec(R"sql(UPDATE admin SET
        nombre = CASE WHEN $1 <> '' THEN $1 ELSE nombre END,
        correo = CASE WHEN $2 <> '' THEN $2 ELSE correo END,
        contrasena = CASE WHEN $3 <> '' THEN $3 ELSE contrasena END
        WHERE adminid = $4)sql", 
    params{
        entity.getNombre(),     //param 1
        entity.getCorreo(),     //param 2
        entity.getContrasena(), //param 3
        entity.getId()          //param 4
    });

    txn.commit();
    return r.affected_rows() > 0;
}

bool AdminRepo::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("DELETE FROM admin WHERE adminid = $1",
        params{
            id // parametro 1
        });
    txn.commit();

    return r.affected_rows() > 0;
}