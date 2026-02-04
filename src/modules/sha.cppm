module;

#include <openssl/sha.h>
#include <string>

export module sha;

namespace SHA {
    export std::string sha256(const std::string &str);
}