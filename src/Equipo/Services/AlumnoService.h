#pragma once
#include "Generics/Service.h"
#include "Equipo/Models/AlumnoModel.h"
#include "Equipo/Repositories/AlumnoRepository.h"

class AlumnoService : public Service<AlumnoModel, AlumnoRepository> {

public:

    explicit AlumnoService(AlumnoRepository& repo);

    std::vector<AlumnoModel> findAll() const override;
    AlumnoModel findById(int id) const override;
    int insert(const AlumnoModel& entity) override;
    bool update(const AlumnoModel& entity) override;
    bool remove(int id) override;

    bool validate(const AlumnoModel& entity) override;
};
