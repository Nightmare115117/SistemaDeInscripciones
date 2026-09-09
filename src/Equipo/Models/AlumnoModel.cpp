#include "AlumnoModel.h"

using namespace std;

AlumnoModel::AlumnoModel() : AlumnoModel("", 0, false, "", 0, "", "", "", "", "", 0, 0, 0,0) {}

AlumnoModel::AlumnoModel(const string& nombre) :Entity(0) {
    this->nombre = nombre;
}

AlumnoModel::AlumnoModel(const string& nombre, int idEquipo, bool firmo,
const string& correo, const string& numeroTel, const string& apellidoPaterno,
const string& apellidoMaterno, const string& alergias,
const string& condicionMedica, const string& medicamento, int idContacto, int idUniversidad,
int nivelEstudio, int idPais) : Entity<int>(0) {
    this->nombre = nombre;
    this->idEquipo = idEquipo;
    this->firmoTerminos = firmo;
    this->correo = correo;
    this->numerotel = numeroTel;
    this->apellidoPaterno = apellidoPaterno;
    this->apellidoMaterno = apellidoMaterno;
    this->alergias = alergias;
    this->condicionMedica = condicionMedica;
    this->medicamento = medicamento;
    this->idContacto = idContacto;
    this->idUniversidad = idUniversidad;
    this->nivelStudio = nivelEstudio;
    this->idPais = idPais;
}

const string& AlumnoModel::getNombre() const {
    return nombre;
}

int AlumnoModel::getIdEquipo() const {
    return idEquipo;
}

void AlumnoModel::setIdEquipo(int idEquipo) {
    this->idEquipo = idEquipo;
} 

void AlumnoModel::setNombre(const string& nombre) {
    this->nombre = nombre;
}

bool AlumnoModel::firmoterminos() const {
    return firmoTerminos;
}

const string& AlumnoModel::getCorreo() const {
    return correo;
}

const string& AlumnoModel::getNumeroTel() const {
    return numerotel;
}

const string& AlumnoModel::getApellidoPaterno() const {
    return apellidoPaterno;
}

const string& AlumnoModel::getApellidoMaterno() const {
    return apellidoMaterno;
}

const string& AlumnoModel::getAlergias() const {
    return alergias;
}

const string& AlumnoModel::getCondicionMedica() const {
    return condicionMedica;
}

const string& AlumnoModel::getMedicamento() const {
    return medicamento;
}

int AlumnoModel::getIdContacto() const {
    return idContacto;
}

int AlumnoModel::getIdUniversidad() const {
    return idUniversidad;
}

int AlumnoModel::getNivelEstudio() const {
    return nivelStudio;
}

int AlumnoModel::getIdPais() const {
    return idPais;
}

int AlumnoModel::getEdad() const {
    return edad;
}

bool AlumnoModel::getAceptaCodigoConductaMLH() const {
    return aceptaCodigoConductaMLH;
}

bool AlumnoModel::getAceptaCompartirDatosMLH() const {
    return aceptaCompartirDatosMLH;
}

bool AlumnoModel::getAceptaCorreosMLH() const {
    return aceptaCorreosMLH;
}

bool AlumnoModel::getCorreoVerificado() const {
    return correoVerificado;
}

void AlumnoModel::setFirmoTerminos(bool firmo) {
    this->firmoTerminos = firmo;
}

void AlumnoModel::setCorreo(const string& correo) {
    this->correo = correo;
}

void AlumnoModel::setNumeroTel(const string& tel) {
    this->numerotel = tel;
}

void AlumnoModel::setApellidoPaterno(const string& apellidoPaterno) {
    this->apellidoPaterno = apellidoPaterno;
}

void AlumnoModel::setApellidoMaterno(const string& apellidoMaterno) {
    this->apellidoMaterno = apellidoMaterno;
}

void AlumnoModel::setAlergias(const string& alergias) {
    this->alergias = alergias;
}

void AlumnoModel::setCondicionMedica(const string& condicionMedica) {
    this->condicionMedica = condicionMedica;
}

void AlumnoModel::setMedicamento(const string& medicamento) {
    this->medicamento = medicamento;
}

void AlumnoModel::setIdContacto(int id) {
    this->idContacto = id;
}

void AlumnoModel::setIdUniversidad(int idUniversidad){
    this->idUniversidad = idUniversidad;
}

void AlumnoModel::setNivelEstudio(int id) {
    this->nivelStudio = id;
}

void AlumnoModel::setIdPais(int id) {
    this->idPais = id;
}

void AlumnoModel::setEdad(int edad) {
    this->edad = edad;
}

void AlumnoModel::setAceptaCodigoConductaMLH(bool aceptar) {
    this->aceptaCodigoConductaMLH = aceptar;
}

void AlumnoModel::setAceptaCompartirDatosMLH(bool aceptar) {
    this->aceptaCompartirDatosMLH = aceptar;
}

void AlumnoModel::setAceptaCorreosMLH(bool aceptar) {
    this->aceptaCorreosMLH = aceptar;
}

void AlumnoModel::setCorreoVerificado(bool verificado) {
    this->correoVerificado = verificado;
}