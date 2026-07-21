#pragma once
#include "Generics/Controller.h"
#include "Equipo/Models/EquipoModel.h"
#include "Equipo/Services/EquipoService.h"

class EquipoController : public Controller<EquipoModel, EquipoService> {

public:
    explicit EquipoController(EquipoService& service);

    crow::json::wvalue toJson(const EquipoModel& entidad) const override;
    EquipoModel fromJson(const crow::json::rvalue& json) const override;
};
