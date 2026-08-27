#include "RegistroService.h"
#include "Security/Crypto.h"
#include <stdexcept>

using namespace std;

RegistroService::RegistroService(RegistroRepository& repo)
    : Service<RegistroModel, RegistroRepository>(repo) {}

RegistroService::RegistroService(RegistroRepository& registroRepo,
                                                                 EquipoRepository& equipoRepo,
                                                                 AlumnoRepository& alumnoRepo,
                                                                 ContactoEmergenciaRepository& contactoRepo)
        : Service<RegistroModel, RegistroRepository>(registroRepo),
            equipoRepo(&equipoRepo), alumnoRepo(&alumnoRepo), contactoRepo(&contactoRepo) {}

bool RegistroService::validate(const RegistroModel& entity) {
    if (entity.getIdEquipo() <= 0 && entity.getIdEquipo() != -1) return false;
    return true;
}

vector<RegistroModel> RegistroService::findAll() const {
    return repo.findAll();
}

//throw runtime_error("Registro con el id: " + to_string(id) + " no existe");
//Usar en findById

RegistroModel RegistroService::findById(int id) const {
    if (id <= 0) throw logic_error("Id debe ser positivo");

    return repo.findById(id);
}

int RegistroService::insert(const RegistroModel& entity) {
    if (!validate(entity)) throw logic_error("El id no puede ser 0 ni negativo");
    if (repo.countById() == 47) throw logic_error("No puede haber más de 47 equipos");
    
    return repo.insert(entity);
}

bool RegistroService::update(const RegistroModel& entity) {
    throw logic_error("No se pueden modificar registros existentes");
}

bool RegistroService::remove(int id) {
    if (id <= 0) throw logic_error("El id debe ser positivo");

    return repo.remove(id);;
}

int RegistroService::countById() const {
    return repo.countById();
}

RegistroRepository::CountDTO RegistroService::countStats() const {
    return repo.countStats();
}

int RegistroService::insertRegistroCompleto(const RegistroModel& regis, const AlumnoModel& alumno, const EquipoModel& equipo, const ContactoEmergenciaModel& contacto) {
    return insertRegistroCompleto(regis, equipo, {alumno}, {contacto});
}

int RegistroService::insertRegistroCompleto(RegistroModel registro,
                                            EquipoModel equipo,
                                            vector<AlumnoModel> alumnos,
                                            vector<ContactoEmergenciaModel> contactos) {
    if (!equipoRepo || !alumnoRepo || !contactoRepo) {
        throw logic_error("El servicio de registro no tiene sus repositorios relacionados");
    }
    if (alumnos.empty() || alumnos.size() != contactos.size()) {
        throw invalid_argument("Cada integrante debe tener un contacto de emergencia");
    }

    int equipoId = equipoRepo->insert(equipo);
    int liderId = -1;

    for (size_t index = 0; index < alumnos.size(); ++index) {
        int contactoId = contactoRepo->insert(contactos[index]);
        alumnos[index].setIdEquipo(equipoId);
        alumnos[index].setIdContacto(contactoId);

        AlumnoModel alumno = alumnos[index];
        alumno.setCorreo(AES::encrypt(alumno.getCorreo()));
        alumno.setNumeroTel(AES::encrypt(alumno.getNumeroTel()));
        int alumnoId = alumnoRepo->insert(alumno);
        if (index == 0) liderId = alumnoId;
    }

    equipo.setId(equipoId);
    equipo.setIdLider(liderId);
    equipoRepo->update(equipo);

    registro.setIdEquipo(equipoId);
    insert(registro);
    return equipoId;
}