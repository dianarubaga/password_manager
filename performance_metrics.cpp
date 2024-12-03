#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "include/encryption.h"
#include "include/manager.h"

// Function to benchmark encryption performance
void benchmarkEncryption(const std::string &plaintext, const std::string &key)
{
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
void benchmarkDecryption(const std::vector<unsigned char> &ciphertext, const std::string &key)
{
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

// Function to benchmark password generation
void benchmarkPasswordGeneration(PasswordNS::PasswordManager &manager, int length)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::string password = manager.generatePassword(length);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Password Generation Performance:\n";
    std::cout << "Password Length: " << length << "\n";
    std::cout << "Time Taken: " << duration << " µs\n\n";
}

// Function to benchmark file save and load operations
void benchmarkFileOperations(PasswordNS::PasswordManager &manager)
{
    auto start = std::chrono::high_resolution_clock::now();
    manager.saveCredentials(); // Use public wrapper method
    auto end = std::chrono::high_resolution_clock::now();

    auto save_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "File Save Performance:\n";
    std::cout << "Time Taken: " << save_duration << " µs\n\n";

    start = std::chrono::high_resolution_clock::now();
    manager.loadCredentials(); // Use public wrapper method
    end = std::chrono::high_resolution_clock::now();

    auto load_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "File Load Performance:\n";
    std::cout << "Time Taken: " << load_duration << " µs\n\n";
}

int main()
{
    // Encryption key
    const std::string key = "0123456789abcdef0123456789abcdef";

    // Test input sizes for encryption/decryption
    std::vector<int> inputSizes = {256, 1024, 1048576}; // 256 bytes, 1 KB, 1 MB
    PasswordNS::PasswordManager manager;

    // Set up test data for file operations
    manager.setTestCredentials("test_user", "secure_password");
    for (int i = 0; i < 100; ++i)
    {
        manager.addNewPassword("service" + std::to_string(i), "user" + std::to_string(i), "password" + std::to_string(i));
    }

    for (int size : inputSizes)
    {
        const std::string plaintext(size, 'A'); // Generate test plaintext
        std::cout << "Testing with input size: " << size << " bytes\n";

        // Benchmark encryption
        benchmarkEncryption(plaintext, key);

        // Perform encryption to get ciphertext
        auto ciphertext = EncryptionNS::encrypt(plaintext, key);

        // Benchmark decryption
        benchmarkDecryption(ciphertext, key);

        std::cout << "--------------------------------------\n";
    }

    // Test password generation for different lengths
    std::vector<int> lengths = {8, 16, 32, 64};
    for (int length : lengths)
    {
        benchmarkPasswordGeneration(manager, length);
    }

    // Benchmark file save and load operations
    benchmarkFileOperations(manager);

    return 0;
}