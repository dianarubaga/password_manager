#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <optional> // For std::optional
#include <memory>

using namespace std;

namespace PasswordNS {

// Abstract base class for managing common operations
class BaseManager {
public:
    virtual void encrypt(const string &data) const = 0;      // Pure virtual function for encryption
    virtual bool validate(const string &password) const = 0; // Pure virtual function for validation
    virtual ~BaseManager() noexcept = default;               // Virtual destructor
};

// Derived class for managing passwords
class PasswordManager : public BaseManager {
private:
    vector<pair<string, string>> credentials; // Container for credentials (service, password)
    string username;
    string mainPassword;

    void saveCredentialsToFile();
    inline static const string encryptionKey = "MySecretKey";

public:
    // Rule of 3/5: Constructors, Assignment Operators, Destructor
    PasswordManager();
    PasswordManager(const PasswordManager &other);
    PasswordManager(PasswordManager &&other) noexcept;
    PasswordManager &operator=(const PasswordManager &other);
    PasswordManager &operator=(PasswordManager &&other) noexcept;
    ~PasswordManager() noexcept override;

    // Encapsulation: Getters and Setters
    void setUsername(const string &name) { username = name; }
    const string &getUsername() const { return username; }
    void setMainPassword(const string &password) { mainPassword = password; }
    const string &getMainPassword() const { return mainPassword; }

    void addNewPassword(string serviceName, string serviceUsername, string password);
    void showAllPasswords();
    void deletePassword(string serviceName);
    string generatePassword(int length);
    void useGeneratedPasswordForNewEntry(const string &generatedPassword);
    void saveUserCredentialsToFile();
    bool loadUserCredentialsFromFile();
    void loadCredentialsFromFile();

    [[nodiscard]] std::optional<string> getCredential(const string &serviceName);
    bool hasPassword(const string &serviceName);

    void setTestCredentials(const string &testUsername, const string &testPassword);

    inline size_t getPasswordCount() const { return credentials.size(); }
};

}

#endif