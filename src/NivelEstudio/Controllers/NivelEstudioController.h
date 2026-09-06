#pragma once
#include "Generics/Controller.h"
#include "NivelEstudio/Models/NivelEstudioModel.h"
#include "NivelEstudio/Services/NivelEstudioService.h"

class NivelEstudioController : public Controller<NivelEstudioModel, NivelEstudioService> {
public:
    explicit NivelEstudioController(NivelEstudioService& service);

    crow::json::wvalue toJson(const NivelEstudioModel& entity) const override;
    NivelEstudioModel fromJson(const crow::json::rvalue& json) const override;
};
