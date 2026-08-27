#include "UniversityModel.h"

using namespace std;

UniversityModel::UniversityModel() : UniversityModel("") {}

UniversityModel::UniversityModel(const string& nombre) : Entity<int>(0) {
    this->nombre = nombre;
}

const string& UniversityModel::getNombre() const {
    return nombre;
}

void UniversityModel::setNombre(const string& nombre) {
    this->nombre = nombre;
}