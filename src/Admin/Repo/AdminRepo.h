#pragma once

#include "Generics/Repository.h"
#include "DBConfig/DBConfig.h"
#include "Admin/Model/AdminModel.h"

class AdminRepo : public Repository <AdminModel> {

    DBConfig& dbConfig;

public:

    explicit AdminRepo(DBConfig& dbConfig);

    std::vector<AdminModel> findAll() const override;
    AdminModel findById (int id) const override;
    int insert (const AdminModel& Entity) override;
    bool update (const AdminModel& Entity) override;
    bool remove (int id) override;
}; 