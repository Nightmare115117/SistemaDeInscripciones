#pragma once
#include "Generics/Service.h"
#include "Equipo/Models/ContactoEmergenciaModel.h"
#include "Equipo/Repositories/ContactoEmergenciaRepository.h"

class ContactoEmergenciaService : public Service<ContactoEmergenciaModel, ContactoEmergenciaRepository> {

public:

    explicit ContactoEmergenciaService(ContactoEmergenciaRepository& repo);

    std::vector<ContactoEmergenciaModel> findAll() const override;
    ContactoEmergenciaModel findById(int id) const override;
    int insert(const ContactoEmergenciaModel& entity) override;
    bool update(const ContactoEmergenciaModel& entity) override;
    bool remove(int id) override;

    bool validate(const ContactoEmergenciaModel& entity) override;
};
