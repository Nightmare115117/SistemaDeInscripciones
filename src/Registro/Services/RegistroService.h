#pragma once
#include "Generics/Service.h"
#include "Registro/Models/RegistroModel.h"
#include "Registro/Repositories/RegistroRepository.h"

class RegistroService : public Service <RegistroModel> {

    std::vector<RegistroModel> findAll() const;
    RegistroModel findById(int id) const;
    int insert (const RegistroModel& entity);
    bool update(const RegistroModel& entity);
    bool remove(const RegistroModel& entity);

    bool validate(const RegistroModel& entity);
    bool exist(int id);
};