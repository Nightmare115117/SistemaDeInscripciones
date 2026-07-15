#pragma once
#include "Generics/Controller.h"
#include "Equipo/Models/ContactoEmergenciaModel.h"
#include "Equipo/Services/ContactoEmergenciaService.h"

class ContactoEmergenciaController : public Controller<ContactoEmergenciaModel, ContactoEmergenciaService> {

public:
    explicit ContactoEmergenciaController(ContactoEmergenciaService& service);

    crow::json::wvalue toJson(const ContactoEmergenciaModel& entidad) const override;
    ContactoEmergenciaModel fromJson(const crow::json::rvalue& json) const override;
};
