#pragma once

#include "Generics/Controller.h"
#include "Admin/Model/AdminModel.h"
#include "Admin/Service/AdminService.h"

class AdminController : public Controller<AdminModel, AdminService> {
    
public:

    explicit AdminController(AdminService& service);

    crow::json::wvalue toJson(const AdminModel& entity) const override;
    AdminModel fromJson(const crow::json::rvalue& json) const override;
};