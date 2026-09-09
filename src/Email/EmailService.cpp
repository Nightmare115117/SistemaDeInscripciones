#include "EmailService.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

const string EmailService::RESEND_API_KEY = [] {

    const char* envSecret = getenv("RESEND_API_KEY");

    if (!envSecret || string(envSecret).empty())
    {
        cerr << "RESEND_API_KEY no configurada\n";
        exit(EXIT_FAILURE);
        
    } else {
        cout << "RESEND_API_KEY cargada Correctamente\n";
    }

    return string(envSecret);

}();

const string EmailService::RESEND_FROM_EMAIL = [] {

    const char*  envSecret = getenv("RESEND_FROM_EMAIL");
    if (!envSecret || string(envSecret).empty()) {
        cerr << "RESEND_FROM_EMAIL no configurado\n";
        exit(EXIT_FAILURE);
    } else {
        cout << "RESEND_FROM_EMAIL cargado correctmente\n";
    }

    return string(envSecret);

}();

bool EmailService::sendEmail(const std::string& to, const std::string& subject, const std::string& html) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();

    if (!curl) {
        std::cerr << "No se pudo inicializar CURL\n";
        return false;
    }

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "https://api.resend.com/emails"
    );

    curl_slist* slist = nullptr;

    string text = "Authorization: Bearer " + RESEND_API_KEY;
    slist = curl_slist_append(slist, text.c_str());
    slist = curl_slist_append(slist, "Content-Type: application/json");

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        slist
    );

    json contenido;
    contenido["from"] = RESEND_FROM_EMAIL;
    contenido["to"] = to;
    contenido["subject"] = subject;
    contenido["html"] = html;

    string cont = contenido.dump();

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        cont.c_str()
    );

    CURLcode result = curl_easy_perform(curl);

    if (result == CURLE_OK) {
        long salida;
        curl_easy_getinfo(
            curl,
            CURLINFO_RESPONSE_CODE,
            &salida
        );
        curl_slist_free_all(slist);
        curl_easy_cleanup(curl);
        return (salida >= 200 && salida <= 299);
    } else {
        string salida = curl_easy_strerror(result);
        cout << salida <<endl;
        curl_slist_free_all(slist);
        curl_easy_cleanup(curl);
        return false;
    }
}