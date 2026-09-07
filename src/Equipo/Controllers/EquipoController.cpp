#include "EquipoController.h"
#include <stdexcept>

EquipoController::EquipoController(EquipoService& service, AlumnoRepository& alumnos,
        ContactoEmergenciaRepository& contactos, ProblemaRepository& problemas)
        : Controller<EquipoModel, EquipoService>(service), alumnoRepo(alumnos),
            contactoRepo(contactos), problemaRepo(problemas) {}

crow::json::wvalue EquipoController::toJson(const EquipoModel& entidad) const {
    crow::json::wvalue json;
    json["id"] = std::to_string(entidad.getId());
    json["nombre"] = entidad.getNombre();
    json["problematica"] = problemaRepo.findCodigoById(entidad.getIdProblematica());
    json["estado"] = entidad.getEstado();
    json["registradoEn"] = entidad.getRegistradoEn();
    json["revisadoEn"] = entidad.getRevisadoEn().empty() ? crow::json::wvalue(nullptr) : crow::json::wvalue(entidad.getRevisadoEn());
    json["notas"] = entidad.getNotas().empty() ? crow::json::wvalue(nullptr) : crow::json::wvalue(entidad.getNotas());
    json["liderId"] = std::to_string(entidad.getIdLider());
    std::vector<crow::json::wvalue> integrantes;
    for (const auto& alumno : alumnoRepo.findByEquipoId(entidad.getId())) {
        crow::json::wvalue integrante;
        integrante["id"] = std::to_string(alumno.getId());
        integrante["nombre"] = alumno.getNombre();
        integrante["correo"] = alumno.getCorreo();
        integrante["telefono"] = alumno.getNumeroTel();
        integrante["institucion"] = std::to_string(alumno.getIdUniversidad());
        integrante["alergias"] = alumno.getAlergias();
        if (alumno.getIdContacto() > 0) {
            auto contacto = contactoRepo.findById(alumno.getIdContacto());
            crow::json::wvalue emergencia;
            emergencia["nombre"] = contacto.getNombre();
            emergencia["telefono"] = contacto.getTelefono();
            emergencia["parentesco"] = contacto.getPariente();
            integrante["emergencia"] = std::move(emergencia);
        }
        integrantes.push_back(std::move(integrante));
    }
    json["integrantes"] = std::move(integrantes);
    return json;
}

EquipoModel EquipoController::fromJson(const crow::json::rvalue& json) const {
    if (!json.has("nombre")) {
        throw std::invalid_argument("Falta el campo 'nombre'");
    }
    if (!json.has("idLider")) {
        throw std::invalid_argument("Falta el campo 'idLider'");
    }
    if (!json.has("idProblematica")) {
        throw std::invalid_argument("Falta el campo 'idProblematica'");
    }

    EquipoModel equipo(
        json["nombre"].s(),
        json["idLider"].i(),
        json["idProblematica"].i()
    );
    return equipo;
}
