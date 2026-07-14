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
        alumno.setId(r[0]["idalumno"].as<int>());
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