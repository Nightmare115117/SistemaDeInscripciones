#include "AlumnoRepository.h"
#include "Security/Crypto.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace std;
using namespace pqxx;

namespace {
    std::string safeDecrypt(const std::string& value) {
        if (value.empty()) return "";
        try {
            return AES::decrypt(value);
        } catch (...) {
            return value;
        }
    }
}

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
        idcontacto,
        institucion,
        id_pais,
        edad,
        acepta_codigo_conducta_mlh,
        acepta_compartir_datos_mlh,
        acepta_correos_mlh,
        correo_verificado
    FROM alumnos)sql");

    vector<AlumnoModel> lista;

    for (const auto &fila : r) {
        AlumnoModel alumno(fila["nombre"].as<string>());
        alumno.setId(fila["idalumno"].as<int>());
        alumno.setIdEquipo(fila["idequipo"].as<int>());
        alumno.setFirmoTerminos(fila["firmoterminos"].as<bool>());
        alumno.setCorreo(safeDecrypt(fila["correo"].as<string>()));
        alumno.setNumeroTel(safeDecrypt(fila["numerotel"].as<string>()));
        alumno.setApellidoPaterno(fila["apellidopaterno"].as<string>());
        alumno.setApellidoMaterno(fila["apellidomaterno"].as<string>());
        alumno.setAlergias(fila["alergias"].as<string>());
        alumno.setCondicionMedica(fila["condicion"].as<string>());
        alumno.setMedicamento(fila["medicamento"].as<string>());
        alumno.setIdContacto(fila["idcontacto"].as<int>());
        alumno.setInstitucion(fila["institucion"].is_null() ? "" : fila["institucion"].as<string>());
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
        idcontacto,
        institucion,
        id_pais,
        edad,
        acepta_codigo_conducta_mlh,
        acepta_compartir_datos_mlh,
        acepta_correos_mlh,
        correo_verificado
    FROM alumnos WHERE idalumno = $1)sql", params{id});

    if (r.empty()) {
        throw logic_error("No existe un alumno con el id mencionado");
    }

    AlumnoModel alumno(r[0]["nombre"].as<string>());
    alumno.setId(r[0]["idalumno"].as<int>());
    alumno.setIdEquipo(r[0]["idequipo"].as<int>());
    alumno.setFirmoTerminos(r[0]["firmoterminos"].as<bool>());
    alumno.setCorreo(safeDecrypt(r[0]["correo"].as<string>()));
    alumno.setNumeroTel(safeDecrypt(r[0]["numerotel"].as<string>()));
    alumno.setApellidoPaterno(r[0]["apellidopaterno"].as<string>());
    alumno.setApellidoMaterno(r[0]["apellidomaterno"].as<string>());
    alumno.setAlergias(r[0]["alergias"].as<string>());
    alumno.setCondicionMedica(r[0]["condicion"].as<string>());
    alumno.setMedicamento(r[0]["medicamento"].as<string>());
    alumno.setIdContacto(r[0]["idcontacto"].as<int>());
    alumno.setInstitucion(r[0]["institucion"].is_null() ? "" : r[0]["institucion"].as<string>());
    alumno.setIdPais(r[0]["id_pais"].is_null() ? -1 : r[0]["id_pais"].as<int>());
    alumno.setEdad(r[0]["edad"].is_null() ? 0 : r[0]["edad"].as<int>());
    alumno.setAceptaCodigoConductaMLH(r[0]["acepta_codigo_conducta_mlh"].as<bool>());
    alumno.setAceptaCompartirDatosMLH(r[0]["acepta_compartir_datos_mlh"].as<bool>());
    alumno.setAceptaCorreosMLH(r[0]["acepta_correos_mlh"].as<bool>());
    alumno.setCorreoVerificado(r[0]["correo_verificado"].as<bool>());

    return alumno;
}

vector<AlumnoModel> AlumnoRepository::findByEquipoId(int equipoId) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT
        idalumno, nombre, idequipo, firmoterminos, correo, numerotel,
        apellidopaterno, apellidomaterno, alergias, condicion, medicamento,
        idcontacto, institucion, id_pais, edad,
        acepta_codigo_conducta_mlh, acepta_compartir_datos_mlh, acepta_correos_mlh, correo_verificado
    FROM alumnos WHERE idequipo = $1 ORDER BY idalumno)sql", params{equipoId});

    vector<AlumnoModel> lista;
    for (const auto& fila : r) {
        AlumnoModel alumno(fila["nombre"].as<string>());
        alumno.setId(fila["idalumno"].as<int>());
        alumno.setIdEquipo(fila["idequipo"].as<int>());
        alumno.setFirmoTerminos(fila["firmoterminos"].as<bool>());
        alumno.setCorreo(safeDecrypt(fila["correo"].as<string>()));
        alumno.setNumeroTel(safeDecrypt(fila["numerotel"].as<string>()));
        alumno.setApellidoPaterno(fila["apellidopaterno"].as<string>());
        alumno.setApellidoMaterno(fila["apellidomaterno"].as<string>());
        alumno.setAlergias(fila["alergias"].as<string>());
        alumno.setCondicionMedica(fila["condicion"].as<string>());
        alumno.setMedicamento(fila["medicamento"].as<string>());
        alumno.setIdContacto(fila["idcontacto"].as<int>());
        alumno.setInstitucion(fila["institucion"].is_null() ? "" : fila["institucion"].as<string>());
        lista.push_back(alumno);
    }
    return lista;
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
        idcontacto,
        institucion,
        id_pais,
        edad,
        acepta_codigo_conducta_mlh,
        acepta_compartir_datos_mlh,
        acepta_correos_mlh,
        correo_verificado) 
        VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18) 
        RETURNING idalumno)sql", params{
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
        entity.getInstitucion(),
        entity.getIdPais(),
        entity.getEdad(),
        entity.getAceptaCodigoConductaMLH(),
        entity.getAceptaCompartirDatosMLH(),
        entity.getAceptaCorreosMLH(),
        entity.getCorreoVerificado()
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
        idcontacto = CASE WHEN $11 <> -1 THEN $11 ELSE idcontacto END,
        institucion = CASE WHEN $12 <> '' THEN $12 ELSE institucion END,
        id_pais = CASE WHEN $13 <> -1 THEN $13 ELSE id_pais END,
        edad = CASE WHEN $14 <> 0 THEN $14 ELSE edad END,
        acepta_codigo_conducta_mlh = $15,
        acepta_compartir_datos_mlh = $16,
        acepta_correos_mlh = $17,
        correo_verificado = $18
    WHERE idalumno = $19)sql", params{
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
        entity.getInstitucion(),
        entity.getIdPais(),
        entity.getEdad(),
        entity.getAceptaCodigoConductaMLH(),
        entity.getAceptaCompartirDatosMLH(),
        entity.getAceptaCorreosMLH(),
        entity.getCorreoVerificado(),
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

AlumnoRepository::EquipoCantidadDTO AlumnoRepository::countByIdOrderByGroup(int id) const{
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT E.nombre_equipo AS NombreEquipo, COUNT(A.idalumno) AS NOAlumnos FROM alumnos A, equipo E 
        WHERE A.idequipo = E.idequipo AND
        A.idequipo = $1
        GROUP BY E.nombre_equipo
        ORDER BY E.nombre_equipo)sql", params(
            id //parametro 1
        ));

    if (r.empty()) {
        throw logic_error("Error, id de equipo no encontrado o inexistente");
    }

    AlumnoRepository::EquipoCantidadDTO temp;
    temp.nombre_equipo = (r[0]["NombreEquipo"].as<string>());
    temp.Cantidad = (r[0]["NOAlumnos"].as<int>());

    
    return temp;
}

vector<AlumnoRepository::EquipoCantidadDTO> AlumnoRepository::countByIdOrderByGroupAsList() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    result r = txn.exec(R"sql(SELECT E.nombre_equipo AS NombreEquipo, COUNT(A.idalumno) AS NOAlumnos FROM alumnos A, equipo E 
        WHERE A.idequipo = E.idequipo
        GROUP BY E.nombre_equipo
        ORDER BY E.nombre_equipo)sql");

    vector<AlumnoRepository::EquipoCantidadDTO> lista;
    
    for (const auto& fila : r) {
        AlumnoRepository::EquipoCantidadDTO temp;
        temp.nombre_equipo = (fila["NombreEquipo"].as<string>());
        temp.Cantidad = (fila["NOAlumnos"].as<int>());

        lista.push_back(temp);
    }
    
    return lista;
}