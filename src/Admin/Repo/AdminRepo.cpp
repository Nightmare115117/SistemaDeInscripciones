#include "AdminRepo.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

AdminRepo::AdminRepo(DBConfig& dbConfig) : dbConfig(dbConfig) {}

vector<AdminModel> AdminRepo::findAll() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT adminid, nombre, correo_aes, correo_hmac, contrasena FROM admin)sql");

    vector<AdminModel> lista;

    for (const auto& fila : r) {
        AdminModel admin(fila["nombre"].as<string>());
            admin.setId(fila["adminid"].as<int>());
            admin.setCorreoAES(fila["correo_aes"].as<string>());
            admin.setCorreoHMAC(fila["correo_hmac"].as<string>());
            admin.setContrasena(fila["contrasena"].as<string>());
        lista.push_back(admin);
    }

    return lista;
}

AdminModel AdminRepo::findById(int id) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT adminid, 
        nombre, correo_aes, correo_hmac, 
        contrasena FROM admin WHERE adminid = $1)sql",
        params{
            id // param $1
        });
    
    if (r.empty()) 
        throw logic_error("No existe un Admin con el id mencionado");

    AdminModel admin(r[0]["nombre"].as<string>());
        admin.setId(r[0]["adminid"].as<int>());
        admin.setCorreoAES(r[0]["correo_aes"].as<string>());
        admin.setCorreoHMAC(r[0]["correo_hmac"].as<string>());
        admin.setContrasena(r[0]["contrasena"].as<string>());
    
    return admin;
}

int AdminRepo::insert(const AdminModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec(R"sql(INSERT INTO admin 
        (nombre, 
        correo_aes,
        correo_hmac,
        contrasena) 
        VALUES ($1, $2, $3, $4) 
        RETURNING adminid)sql", params{
        entity.getNombre(),    //param 1
        entity.getCorreoAES(),    //param 2
        entity.getCorreoHMAC(), //param 3
        entity.getContrasena() //param 4
    });

    txn.commit();
    return r[0]["adminid"].as<int>();
}

bool AdminRepo::update(const AdminModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);

    result r = txn.exec(R"sql(UPDATE admin SET
        nombre = CASE WHEN $1 <> '' THEN $1 ELSE nombre END,
        correo_aes = CASE WHEN $2 <> '' THEN $2 ELSE correo_aes END,
        corrwo_hmac = CASE WHEN $3 <> '' THEN $3 ELSE correo_hmac END,
        contrasena = CASE WHEN $4 <> '' THEN $3 ELSE contrasena END
        WHERE adminid = $5)sql", 
    params{
        entity.getNombre(),     //param 1
        entity.getCorreoAES(),     //param 2
        entity.getCorreoHMAC(),     //param 3 
        entity.getContrasena(), //param 4
        entity.getId()          //param 5
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

AdminModel AdminRepo::findByCorreoHMAC (const string& correo) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT adminid, 
        nombre, correo_aes, 
        contrasena FROM admin WHERE correo_hmac = $1)sql",
        params{
            correo // param $1
        });
    
    if (r.empty()) 
        throw logic_error("No existe un Admin con el id mencionado");

    AdminModel admin(r[0]["nombre"].as<string>());
        admin.setId(r[0]["adminid"].as<int>());
        admin.setCorreoAES(r[0]["correo_aes"].as<string>());
        admin.setContrasena(r[0]["contrasena"].as<string>());
    
    return admin;
}