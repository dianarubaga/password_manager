#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "encryption.h"

// Function to benchmark encryption performance
void benchmarkEncryption(const std::string& plaintext, const std::string& key) {
    auto start = std::chrono::high_resolution_clock::now();
    auto ciphertext = EncryptionNS::encrypt(plaintext, key);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double throughput = (plaintext.size() / 1024.0 / 1024.0) / (duration / 1e6); // MB/s

    std::cout << "Encryption Performance:\n";
    std::cout << "Input Size: " << plaintext.size() << " bytes\n";
    std::cout << "Time Taken: " << duration << " µs\n";
    std::cout << "Throughput: " << throughput << " MB/s\n\n";
}

// Function to benchmark decryption performance
void benchmarkDecryption(const std::vector<unsigned char>& ciphertext, const std::string& key) {
    auto start = std::chrono::high_resolution_clock::now();
    auto plaintext = EncryptionNS::decrypt(ciphertext, key);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double throughput = (ciphertext.size() / 1024.0 / 1024.0) / (duration / 1e6); // MB/s

    std::cout << "Decryption Performance:\n";
    std::cout << "Input Size: " << ciphertext.size() << " bytes\n";
    std::cout << "Time Taken: " << duration << " µs\n";
    std::cout << "Throughput: " << throughput << " MB/s\n\n";
}

int main() {
    // Encryption key
    const std::string key = "0123456789abcdef0123456789abcdef";

    // Test input sizes
    std::vector<int> inputSizes = {256, 1024, 1048576}; // 256 bytes, 1 KB, 1 MB

    for (int size : inputSizes) {
        // Generate test data
        const std::string plaintext(size, 'A'); // Repeat 'A' for specified size
        std::cout << "Testing with input size: " << size << " bytes\n";

        // Benchmark encryption
        benchmarkEncryption(plaintext, key);

        // Perform encryption to get ciphertext
        auto ciphertext = EncryptionNS::encrypt(plaintext, key);

        // Benchmark decryption
        benchmarkDecryption(ciphertext, key);

        std::cout << "--------------------------------------\n";
    }

    return 0;
}
