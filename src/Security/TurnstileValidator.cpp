#include "TurnstileValidator.h"
#include <curl/curl.h>
#include <cctype>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

namespace {
    const string SITEVERIFY_URL = "https://challenges.cloudflare.com/turnstile/v0/siteverify";

    size_t writeResponse(char* data, size_t size, size_t count, void* userData) {
        size_t total = size * count;
        string* response = static_cast<string*>(userData);
        response->append(data, total);
        return total;
    }

    string encode(const string& value) {
        static const char* hex = "0123456789ABCDEF";
        string result;

        for (unsigned char character : value) {
            if (isalnum(character) || character == '-' || character == '_' ||
                character == '.' || character == '~') {
                result += static_cast<char>(character);
            } else {
                result += '%';
                result += hex[character >> 4];
                result += hex[character & 0x0F];
            }
        }

        return result;
    }
}

bool CurlTurnstileHttpClient::post(const string& url,
                                   const string& body,
                                   string& response) const {
    static const bool curlInitialized = curl_global_init(CURL_GLOBAL_DEFAULT) == CURLE_OK;
    if (!curlInitialized) return false;

    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(body.size()));
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeResponse);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    CURLcode result = curl_easy_perform(curl);
    long statusCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return result == CURLE_OK && statusCode >= 200 && statusCode <= 299;
}

TurnstileValidator::TurnstileValidator(const string& secretKey,
                                       const string& expectedHostname,
                                       const TurnstileHttpClient& httpClient)
    : secretKey(secretKey), expectedHostname(expectedHostname), httpClient(httpClient) {}

TurnstileValidationStatus TurnstileValidator::validate(const string& token) const {
    if (secretKey.empty()) return TurnstileValidationStatus::NotConfigured;
    if (token.empty() || token.size() > 2048) return TurnstileValidationStatus::Invalid;

    string body = "secret=" + encode(secretKey) + "&response=" + encode(token);
    string response;
    if (!httpClient.post(SITEVERIFY_URL, body, response)) {
        return TurnstileValidationStatus::Unavailable;
    }

    try {
        json result = json::parse(response);
        if (!result.contains("success") || !result["success"].is_boolean()) {
            return TurnstileValidationStatus::Unavailable;
        }
        if (!result["success"].get<bool>()) return TurnstileValidationStatus::Invalid;
        if (!result.contains("action") || result["action"] != "registro") {
            return TurnstileValidationStatus::Invalid;
        }
        if (!expectedHostname.empty() &&
            (!result.contains("hostname") || result["hostname"] != expectedHostname)) {
            return TurnstileValidationStatus::Invalid;
        }
    } catch (const json::exception&) {
        return TurnstileValidationStatus::Unavailable;
    }

    return TurnstileValidationStatus::Valid;
}
