#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <openssl/evp.h>
#include <string>
#include <vector>

namespace EncryptionNS {
    std::vector<unsigned char> encrypt(const std::string &plaintext, const std::string &key);
    std::string decrypt(const std::vector<unsigned char> &ciphertext, const std::string &key);
}

#endif
