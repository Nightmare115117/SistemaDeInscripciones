#include "EmailService.h"
#include <stdexcept>
#include <iostream>

using namespace std;

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

