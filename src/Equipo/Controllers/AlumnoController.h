#pragma once
#include "Generics/Controller.h"
#include "Equipo/Models/AlumnoModel.h"
#include "Equipo/Services/AlumnoService.h"

class AlumnoController : public Controller<AlumnoModel, AlumnoService> {

public:
    explicit AlumnoController(AlumnoService& service);

    crow::json::wvalue toJson(const AlumnoModel& entidad) const override;
    AlumnoModel fromJson(const crow::json::rvalue& json) const override;
};
