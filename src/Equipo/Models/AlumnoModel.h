#pragma once
#include <string>
#include "Generics/Entity.h"

class AlumnoModel : public Entity {

    std::string nombre;
    int idEquipo;
    bool firmoTerminos;
    std::string correo;
    std::string numerotel;
    std::string apellidoPaterno;
    std::string apellidoMaterno;
    std::string alergias;
    std::string condicionMedica;
    std::string medicamento;
    int idContacto;

public:

    AlumnoModel();
    AlumnoModel(const std::string& nombre);
    explicit AlumnoModel(
        const std::string& nombre, int idEquipo, bool firmo, 
        const std::string& correo, const std::string& numeroTel, const std::string& apellidoPaterno,
        const std::string& apellidoMaterno, const std::string& alergias, 
        const std::string& condicionMedica, const std::string& medicamento, 
        int idContacto
    );

    const std::string& getNombre() const;
    int getIdEquipo() const;
    bool firmoterminos() const;
    const std::string& getCorreo() const;
    const std::string& getNumeroTel() const;
    const std::string& getApellidoPaterno() const;
    const std::string& getApellidoMaterno() const;
    const std::string& getAlergias() const;
    const std::string& getCondicionMedica() const;
    const std::string& getMedicamento() const;
    int getIdContacto() const;

    void setNombre(const std::string& nombre);
    void setIdEquipo(int idEquipo);
    void setFirmoTerminos(bool firmo);
    void setCorreo(const std::string& correo);
    void setNumeroTel(const std::string& tel);
    void setApellidoPaterno(const std::string& apellidoPaterno);
    void setApellidoMaterno(const std::string& apellidoMaterno);
    void setAlergias(const std::string& alergias);
    void setCondicionMedica(const std::string& condicionMedica);
    void setMedicamento(const std::string& medicamento);
    void setIdContacto(int id);
};