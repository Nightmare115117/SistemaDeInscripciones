#include "EventoRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>

using namespace pqxx;

EventoModel EventoRepository::find() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    EventoModel item;
    auto row = txn.exec("SELECT cupo, min_integrantes, max_integrantes, registro_abierto FROM evento_configuracion WHERE id = 1").one_row();
    item.setCupo(row[0].as<int>());
    item.setMinIntegrantes(row[1].as<int>());
    item.setMaxIntegrantes(row[2].as<int>());
    item.setRegistroAbierto(row[3].as<bool>());
    for (const auto& problem : txn.exec("SELECT idproblematica::text, coalesce(codigo, ''), nombre, descripcion FROM problematica ORDER BY idproblematica")) {
        item.setProblematicas(item.getProblematicas());
        auto problematicas = item.getProblematicas();
        problematicas.push_back({problem[0].as<std::string>(), problem[1].as<std::string>(), problem[2].as<std::string>(), problem[3].as<std::string>()});
        item.setProblematicas(problematicas);
    }
    return item;
}

EventoModel EventoRepository::update(const EventoModel& item) const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    work txn(conn);
    txn.exec("UPDATE evento_configuracion SET cupo = $1, min_integrantes = $2, max_integrantes = $3, registro_abierto = $4, updated_at = now() WHERE id = 1",
        params{item.getCupo(), item.getMinIntegrantes(), item.getMaxIntegrantes(), item.getRegistroAbierto()});
    txn.commit();
    return find();
}

CorreoEstadoModel EventoRepository::correoEstado() const {
    connection conn(dbConfig.obtenerDatabaseUrl());
    nontransaction txn(conn);
    auto row = txn.exec("SELECT configurado, habilitado, remitente FROM correo_configuracion WHERE id = 1").one_row();
    CorreoEstadoModel item;
    item.setConfigurado(row[0].as<bool>()); item.setHabilitado(row[1].as<bool>()); item.setRemitente(row[2].as<std::string>());
    return item;
}
