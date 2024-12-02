#include "encryption.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <memory> // For smart pointers

namespace EncryptionNS {

// Encrypt Function
std::vector<unsigned char> encrypt(const std::string &plaintext, const std::string &key) {
    const unsigned char *key_data = reinterpret_cast<const unsigned char *>(key.c_str());
    unsigned char iv[16] = {}; // Use a secure random IV in production
    std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    int out_len = 0;

    // Use a unique_ptr with a custom deleter for automatic cleanup of the cipher context
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
    if (!ctx) throw std::runtime_error("Failed to create cipher context");

    EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, key_data, iv);
    EVP_EncryptUpdate(ctx.get(), ciphertext.data(), &out_len,
                      reinterpret_cast<const unsigned char *>(plaintext.c_str()),
                      plaintext.size());
    int total_len = out_len;

    EVP_EncryptFinal_ex(ctx.get(), ciphertext.data() + out_len, &out_len);
    total_len += out_len;
    ciphertext.resize(total_len);

    return ciphertext;
}

// Decrypt Function
std::string decrypt(const std::vector<unsigned char> &ciphertext, const std::string &key) {
    const unsigned char *key_data = reinterpret_cast<const unsigned char *>(key.c_str());
    unsigned char iv[16] = {}; // Use the same IV used for encryption
    std::vector<unsigned char> plaintext(ciphertext.size());
    int out_len = 0;

    // Use a unique_ptr with a custom deleter for automatic cleanup of the cipher context
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
    if (!ctx) throw std::runtime_error("Failed to create cipher context");

    EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, key_data, iv);
    EVP_DecryptUpdate(ctx.get(), plaintext.data(), &out_len, ciphertext.data(), ciphertext.size());
    int total_len = out_len;

    EVP_DecryptFinal_ex(ctx.get(), plaintext.data() + out_len, &out_len);
    total_len += out_len;
    plaintext.resize(total_len);

    return std::string(plaintext.begin(), plaintext.end());
}

} // namespace EncryptionNS
