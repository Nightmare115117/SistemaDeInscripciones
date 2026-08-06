#include <string>

class HMAC {

static std::string HMAC_KEY;

public:

static std::string generate(std::string data);

};