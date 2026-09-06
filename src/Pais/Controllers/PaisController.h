#pragma once
#include "Generics/Controller.h"
#include "Pais/Models/PaisModel.h"
#include "Pais/Services/PaisService.h"

class PaisController : public Controller<PaisModel, PaisService> {
public:
    explicit PaisController(PaisService& service);

    crow::json::wvalue toJson(const PaisModel& entity) const override;
    PaisModel fromJson(const crow::json::rvalue& json) const override;
};
