#pragma once
#include <string>
#include "Generics/Entity.h"

class EquipoModel : public Entity<int> {

    std::string nombre;
    int idLider;
    int idProblematica;
    std::string estado;
    std::string registradoEn;
    std::string revisadoEn;
    std::string notas;

public:

    EquipoModel();
    explicit EquipoModel(const std::string& nombre);
    EquipoModel(const std::string& nombre, int idLider = -1, int idProblematica = -1);

    const std::string& getNombre() const;
    int getIdLider() const;
    int getIdProblematica() const;
    const std::string& getEstado() const;
    const std::string& getRegistradoEn() const;
    const std::string& getRevisadoEn() const;
    const std::string& getNotas() const;

    void setNombre(const std::string& nombre);
    void setIdLider(int idLider);
    void setIdProblematica(int idProblematica);
    void setEstado(const std::string& estado);
    void setRegistradoEn(const std::string& fecha);
    void setRevisadoEn(const std::string& fecha);
    void setNotas(const std::string& notas);
};
