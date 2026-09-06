#include "Cache.h"
#include "Equipo/Models/AlumnoModel.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>

using namespace std;
using namespace nlohmann;

template <>
bool Cache<AlumnoModel>::exist(const string& key){
    auto ruta = filesystem::path("/cache") / key;
    return filesystem::exists(ruta);
}

template<>
bool Cache<AlumnoModel>::isValid(const string& key) {
    if (exist(key)) {
        auto ruta = filesystem::path("/cache") / key;
        auto antiguedad = filesystem::last_write_time(ruta);
        auto ahora = filesystem::file_time_type::clock::now();
        auto tiempo = chrono::duration_cast<chrono::minutes>(ahora - antiguedad);
        return (tiempo.count() <= 10);
    }
    return false;
}

template <>
vector<AlumnoModel> Cache<AlumnoModel>::get(const string& key) {
    if (exist(key)) {
        if (isValid(key)) {
            auto ruta = filesystem::path("/cache") / key;
            auto archivo = ifstream(ruta);
        
            if (archivo.is_open()) {
                auto parser = json::parse(archivo);
                if (parser.is_array()) {
                    vector<AlumnoModel> alumnos;
                    for (const auto& elemento : parser) {
                        AlumnoModel alumno;
                        alumno.setId(elemento["id"].get<int>());
                        alumno.setNombre(elemento["nombre"].get<string>());
                        alumno.setIdEquipo(elemento["idEquipo"].get<int>());
                        alumno.setFirmoTerminos(elemento["firmoTerminos"].get<bool>());
                        alumno.setCorreo(elemento["correo"].get<string>());
                        alumno.setNumeroTel(elemento["numeroTel"].get<string>());
                        alumno.setApellidoPaterno(elemento["apellidoPaterno"].get<string>());
                        alumno.setApellidoMaterno(elemento["apellidoMaterno"].get<string>());
                        alumno.setAlergias(elemento["alergias"].get<string>());
                        alumno.setCondicionMedica(elemento["condicion"].get<string>());
                        alumno.setMedicamento(elemento["medicamento"].get<string>());
                        alumno.setIdContacto(elemento["idContacto"].get<int>());
                        alumno.setIdUniversidad(elemento["idUniversidad"].get<int>());
                        alumno.setNivelEstudio(elemento["nivel_estudio"].get<int>());
                        alumno.setIdPais(elemento["idPais"].get<int>());
                        alumnos.push_back(alumno);
                    }
                        return alumnos;
                } else {
                    cout << "Formato incorrecto" << endl;
                    throw runtime_error("Unxpected format");
                }
            } else {
                cout << "Archivo corrupto o inaccesible" << endl;
                throw runtime_error("Archivo Corrupto o inaccesible");
            }
        } else {
            cout << "Archivo expirado" << endl;
            throw runtime_error("File expired");
        }
    } else {
        cout << "Archivo no encontrado" << endl;
        throw runtime_error("File not found");
    }
} 

template<>
void Cache<AlumnoModel>::set(const string& key, const vector<AlumnoModel>& entitys){
    
    auto ruta = filesystem::path("/cache") / key;
    auto archivo = ofstream(ruta);
    if (archivo.is_open()) {
        json alumnos = json::array();
        for (const AlumnoModel& elemento : entitys) {
            json alumno;
            alumno["id"] = elemento.getId();
            alumno["nombre"] = elemento.getNombre();
            alumno["idEquipo"] = elemento.getIdEquipo();
            alumno["firmoTerminos"] = elemento.firmoterminos();
            alumno["correo"] = elemento.getCorreo();
            alumno["numeroTel"] = elemento.getNumeroTel();
            alumno["apellidoPaterno"] = elemento.getApellidoPaterno();
            alumno["apellidoMaterno"] = elemento.getApellidoMaterno();
            alumno["alergias"] = elemento.getAlergias();
            alumno["condicion"] = elemento.getCondicionMedica();
            alumno["medicamento"] = elemento.getMedicamento();
            alumno["idContacto"] = elemento.getIdContacto();
            alumno["idUniversidad"] = elemento.getIdUniversidad();
            alumno["nivel_estudio"] = elemento.getNivelEstudio();
            alumno["idPais"] = elemento.getIdPais();
            alumnos.push_back(alumno);
        }
        archivo << alumnos.dump(4);
    } else {
        cout << "No se pudo abrir o crear el archivo" << endl;
        throw runtime_error("No se pudo abrir/crear el archivo");
    }
}
