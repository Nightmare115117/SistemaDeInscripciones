#pragma once
#include "Evento/Staff/StaffModel.h"
#include "Generics/Repository.h"
#include "DBConfig/DBConfig.h"
#include <vector>

class StaffRepository : public Repository<StaffModel, std::string> {
    DBConfig& dbConfig;
public:
    explicit StaffRepository(DBConfig& config) : dbConfig(config) {}
    std::vector<StaffModel> findAll() const override;
    StaffModel findById(const std::string& id) const override;
    StaffModel insert(const StaffModel& staff) override;
    StaffModel update(const std::string& id, const StaffModel& staff) const;
    bool update(const StaffModel& staff) override;
    bool remove(const std::string& id) override;
    StaffModel updateAndFetch(const std::string& id, const StaffModel& staff) const;
};
