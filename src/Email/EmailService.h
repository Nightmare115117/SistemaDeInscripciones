#pragma once

#include <string>


class EmailService {

    static const std::string RESEND_API_KEY;
    static const std::string RESEND_FROM_EMAIL;

public:
    static bool sendEmail(
        const std::string& to,
        const std::string& subject,
        const std::string& body
        );
};