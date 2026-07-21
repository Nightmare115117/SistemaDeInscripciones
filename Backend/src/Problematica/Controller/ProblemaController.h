#pragma once
#include "Generics/Controller.h"
#include "Problematica/Model/ProblemaModel.h"
#include "Problematica/Service/ProblemaService.h"

class ProblemaController : public Controller<ProblemaModel, ProblemaService> {

public:
    explicit ProblemaController(ProblemaService& service);

    crow::json::wvalue toJson(const ProblemaModel& entidad) const override;
    ProblemaModel fromJson(const crow::json::rvalue& json) const override;
};
