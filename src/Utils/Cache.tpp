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
            try {
                auto ruta = filesystem::path("/cache") / key;
                auto archivo = ifstream(ruta);
                
                    if (archivo.is_open()) {
                        auto parser = json::parse(archivo);
                        
                    } else {
                    }
                
            } catch (const exception& e) {
            }
        }
    } else {
        cout << "Archivo no encontrado" << endl;
        throw runtime_error("File not found");
    }
} 

template<>
void Cache<AlumnoModel>::set(const string& key, const vector<AlumnoModel>& entitys){

}
