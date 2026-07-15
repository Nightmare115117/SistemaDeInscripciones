#pragma once
#include "Generics/Controller.h"
#include "Registro/Models/RegistroModel.h"
#include "Registro/Services/RegistroService.h"

class RegistroController : public Controller <RegistroModel, RegistroService> {

public:
    explicit RegistroController(RegistroService& service);

    crow::json::wvalue toJson(const RegistroModel& entidad) const override;
    RegistroModel fromJson(const crow::json::rvalue& json) const override;
    void contadorDeRegistros(crow::SimpleApp& app, const RegistroService& service);
};