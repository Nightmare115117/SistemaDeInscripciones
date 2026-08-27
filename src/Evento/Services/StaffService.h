#pragma once
#include "Evento/Staff/StaffModel.h"
#include <stdexcept>
#include "Evento/Repositories/StaffRepository.h"

class StaffService {
    StaffRepository& repo;
public:
    explicit StaffService(StaffRepository& repository);
    std::vector<StaffModel> findAll() const;
    StaffModel insert(const StaffModel& item) const;
    StaffModel update(const std::string& id, const StaffModel& item) const;
    bool remove(const std::string& id) const;
private:
    static void validate(const StaffModel& item);
    static void validateId(const std::string& id);
};
