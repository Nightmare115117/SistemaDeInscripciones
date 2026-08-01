#include "PasswodHasher.h"

#include <bcrypt/bcrypt.h>
#include <stdexcept>

using namespace std;

PasswordHasher::PasswordHasher(int costo)
    : costo(costo)
{
    if (costo < 4 || costo > 31)
        throw std::invalid_argument("Costo de bcrypt fuera de rango");
}

string PasswordHasher::hash(const string& password) const{

    char salt [BCRYPT_HASHSIZE];
    char hash [BCRYPT_HASHSIZE];

    int resultado = bcrypt_gensalt(costo, salt);

    if (resultado != 0)
        throw runtime_error("No se pudo generar el salt correctamente");


    resultado = bcrypt_hashpw(password.c_str(), salt, hash);

    if (resultado != 0) 
        throw runtime_error("El hash no se pudo generar correctamente");
    
    return string(hash);
}

bool PasswordHasher::verificar(const string& passwordOriginal, const string& passwordHash) const{

    int resultado = bcrypt_checkpw(passwordOriginal.c_str(), passwordHash.c_str());

    if (resultado == -1)
        throw runtime_error("No se pudo verificar la contraseña");

    return resultado == 0;
}