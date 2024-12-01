#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <optional> // For std::optional
#include <memory>
#include <filesystem> // For file system operations
#include "Huffman-Encoding/Huffman_C/huffman.h" // Include Huffman Encoding library

namespace PasswordNS
{

    // Abstract base class for managing common operations
    class BaseManager
    {
    public:
        virtual void encrypt(const std::string &data) const = 0;      // Pure virtual function for encryption
        virtual bool validate(const std::string &password) const = 0; // Pure virtual function for validation
        virtual ~BaseManager() noexcept = default;                    // Virtual destructor
    };

    // Derived class for managing passwords
    class PasswordManager : public BaseManager
    {
    private:
        std::vector<std::pair<std::string, std::string>> credentials; // Container for credentials (service, password)
        std::string username;
        std::string mainPassword;

        void saveCredentialsToFile();
        void compressOnExit(); // Compress credentials on exit
        static const std::string encryptionKey; // Declare the encryption key

    public:
        // Rule of 3/5: Constructors, Assignment Operators, Destructor
        PasswordManager();
        PasswordManager(const PasswordManager &other);
        PasswordManager(PasswordManager &&other) noexcept;
        PasswordManager &operator=(const PasswordManager &other);
        PasswordManager &operator=(PasswordManager &&other) noexcept;
        ~PasswordManager() noexcept override;

        // Pure virtual function overrides
        void encrypt(const std::string &data) const override;      // Encryption implementation
        bool validate(const std::string &password) const override; // Validation implementation

        // Encapsulation: Getters and Setters
        void setUsername(const std::string &name) { username = name; }
        const std::string &getUsername() const { return username; }
        void setMainPassword(const std::string &password) { mainPassword = password; }
        const std::string &getMainPassword() const { return mainPassword; }

        void addNewPassword(std::string serviceName, std::string serviceUsername, std::string password);
        void showAllPasswords();
        void deletePassword(std::string serviceName);
        std::string generatePassword(int length);
        void useGeneratedPasswordForNewEntry(const std::string &generatedPassword);
        void saveUserCredentialsToFile();
        bool loadUserCredentialsFromFile(); // Handles decompression if necessary
        void loadCredentialsFromFile();

        [[nodiscard]] std::optional<std::string> getCredential(const std::string &serviceName) const;
        bool hasPassword(const std::string &serviceName) const;

        void setTestCredentials(const std::string &testUsername, const std::string &testPassword);

        inline size_t getPasswordCount() const { return credentials.size(); }

        // Handles user exit and compresses data
        void handleExit();
    };

} // namespace PasswordNS

#endif
