#include "AlumnoController.h"
#include <stdexcept>

AlumnoController::AlumnoController(AlumnoService& service)
    : Controller<AlumnoModel, AlumnoService>(service) {}

crow::json::wvalue AlumnoController::toJson(const AlumnoModel& entidad) const {
    crow::json::wvalue json;
    json["id"] = entidad.getId();
    json["nombre"] = entidad.getNombre();
    json["idEquipo"] = entidad.getIdEquipo();
    json["firmoTerminos"] = entidad.firmoterminos();
    json["correo"] = entidad.getCorreo();
    json["numeroTel"] = entidad.getNumeroTel();
    json["apellidoPaterno"] = entidad.getApellidoPaterno();
    json["apellidoMaterno"] = entidad.getApellidoMaterno();
    json["alergias"] = entidad.getAlergias();
    json["condicion"] = entidad.getCondicionMedica();
    json["medicamento"] = entidad.getMedicamento();
    json["idContacto"] = entidad.getIdContacto();
    return json;
}

AlumnoModel AlumnoController::fromJson(const crow::json::rvalue& json) const {
    if (!json.has("nombre")) {
        throw std::invalid_argument("Falta el campo 'nombre'");
    }
    if (!json.has("idEquipo")) {
        throw std::invalid_argument("Falta el campo 'idEquipo'");
    }
    if (!json.has("correo")) {
        throw std::invalid_argument("Falta el campo 'correo'");
    }

    AlumnoModel alumno;
    alumno.setNombre(json["nombre"].s());
    alumno.setIdEquipo(json["idEquipo"].i());
    alumno.setFirmoTerminos(json.has("firmoTerminos") ? json["firmoTerminos"].b() : false);
    alumno.setCorreo(json["correo"].s());

    if (json.has("numeroTel")) {
        alumno.setNumeroTel(json["numeroTel"].s());
    } else {
        alumno.setNumeroTel("");
    }

    if (json.has("apellidoPaterno")) {
        alumno.setApellidoPaterno(json["apellidoPaterno"].s());
    } else {
        alumno.setApellidoPaterno("");
    }

    if (json.has("apellidoMaterno")) {
        alumno.setApellidoMaterno(json["apellidoMaterno"].s());
    } else {
        alumno.setApellidoMaterno("");
    }

    if (json.has("alergias")) {
        alumno.setAlergias(json["alergias"].s());
    } else {
        alumno.setAlergias("");
    }

    if (json.has("condicion")) {
        alumno.setCondicionMedica(json["condicion"].s());
    } else {
        alumno.setCondicionMedica("");
    }

    if (json.has("medicamento")) {
        alumno.setMedicamento(json["medicamento"].s());
    } else {
        alumno.setMedicamento("");
    }

    alumno.setIdContacto(json.has("idContacto") ? json["idContacto"].i() : -1);

    return alumno;
}
