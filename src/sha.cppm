module;

#include <openssl/sha.h>
#include <string>
#include <array>
#include <span>
#include <sstream>
#include <iomanip>
#include <iostream>

module sha;

namespace SHA {
    std::string sha256(std::string_view str) {
        std::array<unsigned char, SHA256_DIGEST_LENGTH> hash;

        auto bytes = std::as_bytes(std::span{str.data(), str.size()});

        SHA256(reinterpret_cast<const unsigned char*>(bytes.data()),
               bytes.size(),
               hash.data());

        std::string result;
        for (unsigned char byte : hash) {
            std::format_to(std::back_inserter(result), "{:02x}", byte);
        }

        std::cout << "SHA-256 hash computed: " << result << std::endl;
        return result;
    }
}