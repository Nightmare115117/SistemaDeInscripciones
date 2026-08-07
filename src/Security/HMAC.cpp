#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include "HMAC.h"
#include <openssl/hmac.h>

using namespace std;

string HMACsecurity::HMAC_KEY = [] {
    const char* envSecret = getenv("HMAC_KEY");

    if (!envSecret || string(envSecret).empty())
    {
        cerr << "HMAC_KEY no configurada\n";
        exit(EXIT_FAILURE);
        
    } else {
        cout << "HMAC_KEY cargada Correctamente\n";
    }

    return string(envSecret);
}();

string HMACsecurity::generate(const string& dato){
    unsigned char* buffer = nullptr;
    long longitud = 0;
    vector<unsigned char> resultado(32);
    unsigned int out = 0;
    try {

        if (HMAC_KEY.empty( ) || HMAC_KEY.size() != 64)
            throw runtime_error("La key no debe estar vacia");

        if (dato.empty())
            throw logic_error("El dato no puede estar vacio");

        buffer = OPENSSL_hexstr2buf(HMAC_KEY.c_str(), &longitud);

        if (buffer == nullptr || longitud != 32)
            throw runtime_error("No se pudo obtener el buffer");

        unsigned char* salida = nullptr;
        salida = HMAC(EVP_sha256(), buffer, longitud, reinterpret_cast<const unsigned char*>(dato.c_str()), dato.size(), resultado.data(), &out);
        
        if (salida == nullptr)
            throw runtime_error("No se pudo generar la salida de HMAC");

        OPENSSL_free(buffer);

        return HMACsecurity::ByteToHex(resultado);
    } catch(const exception& e) {
        if (buffer != nullptr)
            OPENSSL_free(buffer);
        
        cerr<< e.what() <<endl;
        throw runtime_error("Ocurrio un error inesperado");
    }
}

string HMACsecurity::ByteToHex(vector<unsigned char> entrada){ 
    string resultado = "";
    
    for (const auto& byte : entrada) {
        int valor = byte;
        stringstream stream;
        stream << hex << setw(2) << setfill('0') << valor;
        resultado.append(stream.str());
    }

    if (resultado == "" || resultado.size() != 64)
        throw runtime_error("No se pudo convertir los bytes a hexadecimal");

    return resultado;
}