#pragma once
#include "Evento/Staff/StaffModel.h"
#include <stdexcept>
#include "Evento/Repositories/StaffRepository.h"
#include "Generics/Service.h"

class StaffService : public Service<StaffModel, StaffRepository, std::string> {
public:
    explicit StaffService(StaffRepository& repository);
    std::vector<StaffModel> findAll() const override;
    StaffModel findById(const std::string& id) const override;
    StaffModel insert(const StaffModel& item) override;
    bool update(const StaffModel& item) override;
    bool remove(const std::string& id) override;
    StaffModel updateAndFetch(const std::string& id, const StaffModel& item) const;
private:
    static void validate(const StaffModel& item);
    static void validateId(const std::string& id);
};
