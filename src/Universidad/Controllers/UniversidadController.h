#pragma once
#include "Generics/Controller.h"
#include "Universidad/Models/UniversityModel.h"
#include "Universidad/Services/UniversityService.h"

class UniversidadController : public Controller<UniversityModel, UniversityService> {

public:
    explicit UniversidadController(UniversityService& service);

    crow::json::wvalue toJson(const UniversityModel& entidad) const override;
    UniversityModel fromJson(const crow::json::rvalue& json) const override;
};
