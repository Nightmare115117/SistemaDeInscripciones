#pragma once

#include "Admin/Repo/AdminRepo.h"
#include "Admin/Model/AdminModel.h"
#include "Generics/Service.h"

class AdminService : public Service <AdminModel, AdminRepo> {

public:

    explicit AdminService(AdminRepo& repo);

    std::vector<AdminModel> findAll() const override;
    AdminModel findById(int id) const override;
    int insert(const AdminModel& entity) override;
    bool update(const AdminModel& etity) override;
    bool remove(int id) override;

    bool validate(const AdminModel& entity) override;
};