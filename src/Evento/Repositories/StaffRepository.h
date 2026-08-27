#pragma once
#include "Evento/Staff/StaffModel.h"
#include "DBConfig/DBConfig.h"
#include <vector>

class StaffRepository {
    DBConfig& dbConfig;
public:
    explicit StaffRepository(DBConfig& config) : dbConfig(config) {}
    std::vector<StaffModel> findAll() const;
    StaffModel insert(const StaffModel& staff) const;
    StaffModel update(const std::string& id, const StaffModel& staff) const;
    bool remove(const std::string& id) const;
};
