#include <string>

class HMACsecurity {

    static std::string HMAC_KEY;
    static std::string ByteToHex(std::vector<unsigned char> entrada);

public:

    static std::string generate(const std::string& data);

};