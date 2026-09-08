#pragma once

#include <string>

enum class TurnstileValidationStatus {
    Valid,
    Invalid,
    NotConfigured,
    Unavailable
};

class TurnstileHttpClient {
public:
    virtual ~TurnstileHttpClient() = default;

    virtual bool post(const std::string& url,
                      const std::string& body,
                      std::string& response) const = 0;
};

class CurlTurnstileHttpClient : public TurnstileHttpClient {
public:
    bool post(const std::string& url,
              const std::string& body,
              std::string& response) const override;
};

class TurnstileValidator {
    std::string secretKey;
    std::string expectedHostname;
    const TurnstileHttpClient& httpClient;

public:
    TurnstileValidator(const std::string& secretKey,
                       const std::string& expectedHostname,
                       const TurnstileHttpClient& httpClient);

    TurnstileValidationStatus validate(const std::string& token) const;
};
