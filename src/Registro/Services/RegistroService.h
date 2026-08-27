#pragma once
#include "Generics/Service.h"
#include "Registro/Models/RegistroModel.h"
#include "Registro/Repositories/RegistroRepository.h"
#include "Equipo/Models/AlumnoModel.h"
#include "Equipo/Models/ContactoEmergenciaModel.h"
#include "Equipo/Models/EquipoModel.h"
#include "Equipo/Repositories/EquipoRepository.h"
#include "Equipo/Repositories/AlumnoRepository.h"
#include "Equipo/Repositories/ContactoEmergenciaRepository.h"
#include <vector>

class RegistroService : public Service <RegistroModel, RegistroRepository> {

public:

    explicit RegistroService(RegistroRepository& repo);
    RegistroService(RegistroRepository& registroRepo,
                    EquipoRepository& equipoRepo,
                    AlumnoRepository& alumnoRepo,
                    ContactoEmergenciaRepository& contactoRepo);

    std::vector<RegistroModel> findAll() const;
    RegistroModel findById(int id) const;
    int insert (const RegistroModel& entity);
    bool update(const RegistroModel& entity);
    bool remove(int id);
    int countById() const;
    RegistroRepository::CountDTO countStats() const;

    bool validate(const RegistroModel& entity);

    int insertRegistroCompleto(const RegistroModel& regis, const AlumnoModel& alumno, const EquipoModel& equipo, const ContactoEmergenciaModel& contacto);
    int insertRegistroCompleto(RegistroModel registro,
                               EquipoModel equipo,
                               std::vector<AlumnoModel> alumnos,
                               std::vector<ContactoEmergenciaModel> contactos);

private:
    EquipoRepository* equipoRepo = nullptr;
    AlumnoRepository* alumnoRepo = nullptr;
    ContactoEmergenciaRepository* contactoRepo = nullptr;
};