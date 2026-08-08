#pragma once

#include "Admin/Repo/AdminRepo.h"
#include "Admin/Model/AdminModel.h"
#include "Generics/Service.h"
#include "Security/PasswodHasher.h"

class AdminService : public Service <AdminModel, AdminRepo> {

    const PasswordHasher& hasher;

public:

    explicit AdminService(AdminRepo& repo, PasswordHasher& hasher);

    std::vector<AdminModel> findAll() const override;
    AdminModel findById(int id) const override;
    int insert(const AdminModel& entity) override;
    bool update(const AdminModel& etity) override;
    bool remove(int id) override;
    bool validate(const AdminModel& entity) override;

    AdminModel login(const AdminModel& entity) const;

 
};