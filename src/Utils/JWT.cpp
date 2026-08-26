#include "JWT.h"

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/defaults.h>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
using namespace jwt;

using JWTBuilder = jwt::builder<jwt::default_clock, jwt::traits::nlohmann_json>;
using JWTDecoded = jwt::decoded_jwt<jwt::traits::nlohmann_json>;
using JWTVerifier = jwt::verifier<jwt::default_clock, jwt::traits::nlohmann_json>;

string JWT::SECRET = [] {
    const char* envSecret = getenv("JWT_SECRET");

    if (!envSecret || string(envSecret).empty())
    {
        cerr << "AES_KEY no configurada\n";
        exit(EXIT_FAILURE);
        
    } else {
        cout << "JWT_SECRET cargada Correctamente\n";
    }

    return std::string(envSecret);
}();

string JWT::generarToken(int adminId, const string& username){
    if (SECRET.empty()) 
        throw runtime_error("No se encontro el JWT_KEY");

    auto token = create()
        .set_type("JWT")
        .set_issuer("mi_api")
        .set_payload_claim("adminId", claim(adminId))
        .set_payload_claim("username", claim(username))
        .set_payload_claim("role", claim(string("admin")))
        .set_issued_at(chrono::system_clock::now())
        .set_expires_at(chrono::system_clock::now() + chrono::hours(24))
        .sign(algorithm::hs256{SECRET});
        return token;
}

JWT::TokenInfo JWT::validarToken(const string& token) {
    
    bool firmaValidada = false;
    bool expValidada = false;
    bool payloadValidado = false;
    
    try {
        
        if (SECRET.empty()) 
            throw runtime_error("No se encontro el JWT_KEY");

        if (token.empty()) throw logic_error("No hay un token existente");

        auto tokenDecode = decode(token);

        auto verifier = verify()
            .allow_algorithm(algorithm::hs256{SECRET})
            .with_issuer("mi_api");

        verifier.verify(tokenDecode);
        firmaValidada = true;

        verifier = verify()
            .expires_at_leeway(30);

        verifier.verify(tokenDecode);
        expValidada = true;
        
        if (!tokenDecode.has_payload_claim("adminId"))
            throw logic_error("No tiene el payload AdminId"); 

        if (!tokenDecode.has_payload_claim("username"))
            throw logic_error("No tiene el payload username"); 

        if (!tokenDecode.has_payload_claim("role"))
            throw logic_error("No tiene el payload role");

        payloadValidado = true;

        TokenInfo tokenValido;

            tokenValido.valido = true;
            tokenValido.adminId = stoi(tokenDecode.get_payload_claim("adminId").as_string());
            tokenValido.username = tokenDecode.get_payload_claim("username").as_string();
            tokenValido.role = tokenDecode.get_payload_claim("role").as_string();
            tokenValido.error = NONE;

        return tokenValido;

    } catch (const exception& e) {

        JWTError error = UNKNOWN;

        if (firmaValidada) {
            if (!expValidada) {
                error = EXPIRED;
            } else {
                if (!payloadValidado) {
                    error = MISSING_CLAIM;
                }
            }
        } else {
            error = INVALID_SIGNATURE;
        }

        TokenInfo tokenInvalido;

            tokenInvalido.valido = false;
            tokenInvalido.adminId = -1;
            tokenInvalido.username = "";
            tokenInvalido.role = "";
            tokenInvalido.error = error;

        return tokenInvalido;

    }
}