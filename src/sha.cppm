module;

#include <openssl/sha.h>
#include <string>

module sha;

namespace SHA {
    std::string sha256(const std::string &str) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)str.c_str(), str.length(), hash);
        
        std::string result;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            char buf[3];
            sprintf(buf, "%02x", hash[i]);
            result += buf;
        }
        return result;
    }
}