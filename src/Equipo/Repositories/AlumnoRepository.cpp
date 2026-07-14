#include "AlumnoRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

AlumnoRepository::AlumnoRepository(DBConfig& dbConfig) : dbConfig(dbConfig) {}

vector<AlumnoModel> AlumnoRepository::findAll() const{
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT
        idalumno,
        nombre,
        idequipo,
        firmoterminos,
        correo,
        numerotel,
        apellidopaterno,
        apellidomaterno,
        alergias,
        condicion,
        medicamento,
        idcontacto
    FROM alumnos)sql");

    vector<AlumnoModel> lista;

    for(auto fila : r) {
        AlumnoModel alumno(fila["nombre"].as<string>());
        alumno.setId(fila["idalumno"].as<int>());
        alumno.setIdEquipo(fila["idequipo"].as<int>());
        alumno.setFirmoTerminos(fila["firmoterminos"].as<bool>());
        alumno.setCorreo(fila["correo"].as<string>());
        alumno.setNumeroTel(fila["numerotel"].as<string>());
        alumno.setApellidoPaterno(fila["apellidopaterno"].as<string>());
        alumno.setApellidoMaterno(fila["apellidomaterno"].as<string>());
        alumno.setAlergias(fila["alergias"].as<string>());
        alumno.setCondicionMedica(fila["condicion"].as<string>());
        alumno.setMedicamento(fila["medicamento"].as<string>());
        alumno.setIdContacto(fila["idcontacto"].as<int>());
        lista.push_back(alumno);
    }

    return lista;
}

AlumnoModel AlumnoRepository::findById(int id) const{
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT
        idalumno,
        nombre,
        idequipo,
        firmoterminos,
        correo,
        numerotel,
        apellidopaterno,
        apellidomaterno,
        alergias,
        condicion,
        medicamento,
        idcontacto
    FROM alumnos WHERE idalumno = $1)sql", params{id});

    if (r.empty()) {
        throw logic_error("No existe un alumno con el id mencionado");
    }

    AlumnoModel alumno(r[0]["nombre"].as<string>());
    alumno.setId(r[0]["idalumno"].as<int>());
    alumno.setIdEquipo(r[0]["idequipo"].as<int>());
    alumno.setFirmoTerminos(r[0]["firmoterminos"].as<bool>());
    alumno.setCorreo(r[0]["correo"].as<string>());
    alumno.setNumeroTel(r[0]["numerotel"].as<string>());
    alumno.setApellidoPaterno(r[0]["apellidopaterno"].as<string>());
    alumno.setApellidoMaterno(r[0]["apellidomaterno"].as<string>());
    alumno.setAlergias(r[0]["alergias"].as<string>());
    alumno.setCondicionMedica(r[0]["condicion"].as<string>());
    alumno.setMedicamento(r[0]["medicamento"].as<string>());
    alumno.setIdContacto(r[0]["idcontacto"].as<int>());

    return alumno;
}

int AlumnoRepository::insert(const AlumnoModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec(R"sql(INSERT INTO alumnos 
        (nombre, 
        idequipo, 
        firmoterminos, 
        correo, 
        numerotel, 
        apellidopaterno, 
        apellidomaterno, 
        alergias, 
        condicion, 
        medicamento, 
        idcontacto) 
        VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11) 
        RETURNING idalumno)sql", params{
        entity.getNombre(), // parametro 1
        entity.getIdEquipo(), // parametro 2
        entity.firmoterminos(), // parametro 3
        entity.getCorreo(), // parametro 4
        entity.getNumeroTel(), // parametro 5
        entity.getApellidoPaterno(), // parametro 6
        entity.getApellidoMaterno(), // parametro 7
        entity.getAlergias(), // parametro 8
        entity.getCondicionMedica(), // parametro 9
        entity.getMedicamento(), // parametro 10
        entity.getIdContacto() // parametro 11
    });

    txn.commit();
    return r[0]["idalumno"].as<int>();
}

bool AlumnoRepository::update(const AlumnoModel& entity) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);

    result r = txn.exec(R"sql(UPDATE alumnos SET
        nombre = CASE WHEN $1 <> '' THEN $1 ELSE nombre END,
        idequipo = CASE WHEN $2 <> -1 THEN $2 ELSE idequipo END,
        firmoterminos = $3,
        correo = CASE WHEN $4 <> '' THEN $4 ELSE correo END,
        numerotel = CASE WHEN $5 <> '' THEN $5 ELSE numerotel END,
        apellidopaterno = CASE WHEN $6 <> '' THEN $6 ELSE apellidopaterno END,
        apellidomaterno = CASE WHEN $7 <> '' THEN $7 ELSE apellidomaterno END,
        alergias = CASE WHEN $8 <> '' THEN $8 ELSE alergias END,
        condicion = CASE WHEN $9 <> '' THEN $9 ELSE condicion END,
        medicamento = CASE WHEN $10 <> '' THEN $10 ELSE medicamento END,
        idcontacto = CASE WHEN $11 <> -1 THEN $11 ELSE idcontacto END
    WHERE idalumno = $12)sql", params{
        entity.getNombre(),
        entity.getIdEquipo(),
        entity.firmoterminos(),
        entity.getCorreo(),
        entity.getNumeroTel(),
        entity.getApellidoPaterno(),
        entity.getApellidoMaterno(),
        entity.getAlergias(),
        entity.getCondicionMedica(),
        entity.getMedicamento(),
        entity.getIdContacto(),
        entity.getId()
    });

    txn.commit();
    return r.affected_rows() > 0;
}

bool AlumnoRepository::remove(int id) {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    result r = txn.exec("DELETE FROM alumnos WHERE idalumno = $1",
        params{
            id // parametro 1
        });
    txn.commit();

    return r.affected_rows() > 0;
}