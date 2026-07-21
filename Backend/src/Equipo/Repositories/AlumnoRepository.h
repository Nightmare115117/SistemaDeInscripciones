#pragma once
#include "Generics/Repository.h"
#include "Equipo/Models/AlumnoModel.h"
#include "DBConfig/DBConfig.h"

class AlumnoRepository : public Repository <AlumnoModel> {

DBConfig& dbConfig;

public:

    explicit AlumnoRepository(DBConfig& dbConfig);

    std::vector<AlumnoModel> findAll() const override;
    AlumnoModel findById (int id) const override;
    int insert (const AlumnoModel& entity) override;
    bool update (const AlumnoModel& entity) override;
    bool remove (int id) override;

    struct EquipoCantidadDTO {
    int Cantidad;
    std::string nombre_equipo;
    };

    EquipoCantidadDTO countByIdOrderByGroup(int id) const;
    std::vector<EquipoCantidadDTO> countByIdOrderByGroupAsList() const;

};