#include "manager.h"
#include "encryption.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>

using namespace PasswordNS;

namespace
{

    // Test case for adding a new password
    TEST(PasswordManagerTestSuite, AddNewPassword)
    {
        PasswordManager pm;
        pm.setTestCredentials("testUser", "testPassword"); // No login required
        pm.addNewPassword("email", "user@example.com", "password123");

        auto encryptedPasswordHex = pm.getCredential("email").value();
        std::vector<unsigned char> encryptedPassword;
        for (size_t i = 0; i < encryptedPasswordHex.size(); i += 2)
        {
            encryptedPassword.push_back(static_cast<unsigned char>(std::stoi(encryptedPasswordHex.substr(i, 2), nullptr, 16)));
        }
        auto decryptedPassword = EncryptionNS::decrypt(encryptedPassword, PasswordManager::getEncryptionKey());

        EXPECT_EQ(decryptedPassword, "password123");
    }

    // Test case for deleting a password
    TEST(PasswordManagerTestSuite, DeletePassword)
    {
        PasswordManager pm;
        pm.setTestCredentials("testUser", "testPassword"); // No login required
        pm.addNewPassword("bank", "user1", "securePassword");
        pm.deletePassword("bank");

        EXPECT_FALSE(pm.hasPassword("bank"));
    }

    // Test case for checking if a password can be added and retrieved after deletion
    TEST(PasswordManagerTestSuite, AddAfterDelete)
    {
        PasswordManager pm;
        pm.setTestCredentials("testUser", "testPassword"); // No login required
        pm.addNewPassword("bank", "user1", "securePassword");
        pm.deletePassword("bank");
        pm.addNewPassword("bank", "user1", "newPassword123");

        auto encryptedPasswordHex = pm.getCredential("bank").value();
        std::vector<unsigned char> encryptedPassword;
        for (size_t i = 0; i < encryptedPasswordHex.size(); i += 2)
        {
            encryptedPassword.push_back(static_cast<unsigned char>(std::stoi(encryptedPasswordHex.substr(i, 2), nullptr, 16)));
        }
        auto decryptedPassword = EncryptionNS::decrypt(encryptedPassword, PasswordManager::getEncryptionKey());

        EXPECT_EQ(decryptedPassword, "newPassword123");
    }

    // Additional test for validating passwords
    TEST(PasswordManagerTestSuite, ValidatePassword)
    {
        PasswordManager pm;

        EXPECT_TRUE(pm.validate("StrongPassword123!"));
        EXPECT_FALSE(pm.validate("short"));
        EXPECT_FALSE(pm.validate("weakpass"));
    }

    // Additional test for generating random passwords
    TEST(PasswordManagerTestSuite, GenerateRandomPassword)
    {
        PasswordManager pm;
        std::string password = pm.generatePassword(12);

        EXPECT_EQ(password.length(), 12);

        EXPECT_NE(password.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), std::string::npos);
        EXPECT_NE(password.find_first_of("abcdefghijklmnopqrstuvwxyz"), std::string::npos);
        EXPECT_NE(password.find_first_of("0123456789"), std::string::npos);
        EXPECT_NE(password.find_first_of("!@#$%^&*()"), std::string::npos);
    }

    // Additional test for retrieving all credentials
    TEST(PasswordManagerTestSuite, RetrieveAllCredentials)
    {
        PasswordManager pm;
        pm.setTestCredentials("testUser", "testPassword");

        pm.addNewPassword("email", "user1@example.com", "password123");
        pm.addNewPassword("social", "user2", "socialPass!");

        auto credentials = pm.getAllDecryptedCredentials();
        EXPECT_EQ(credentials.size(), 2);
        EXPECT_EQ(credentials[0].first, "email");
        EXPECT_EQ(credentials[1].first, "social");
    }
    TEST(PasswordManagerTestSuite, AddEmptyPassword)    
    {
        PasswordManager pm;
        pm.setTestCredentials("testUser", "testPassword");
    
        // Adding an entry with an empty password
        EXPECT_THROW(pm.addNewPassword("service", "username", ""), std::invalid_argument);

        // Adding an entry with an empty username
        EXPECT_THROW(pm.addNewPassword("service", "", "password"), std::invalid_argument);

        // Adding an entry with an empty service name
        EXPECT_THROW(pm.addNewPassword("", "username", "password"), std::invalid_argument);
    }
    TEST(PasswordManagerTestSuite, AddMaxLengthPassword)
    {
        PasswordManager pm;
        pm.setTestCredentials("testUser", "testPassword");

        std::string maxServiceName(255, 'a');  // 255 'a's
        std::string maxUsername(255, 'b');    // 255 'b's
        std::string maxPassword(255, 'c');    // 255 'c's

        pm.addNewPassword(maxServiceName, maxUsername, maxPassword);
        
        auto credentials = pm.getCredential(maxServiceName);
        EXPECT_TRUE(credentials.has_value());

        auto encryptedPasswordHex = credentials.value();
        std::vector<unsigned char> encryptedPassword;
        for (size_t i = 0; i < encryptedPasswordHex.size(); i += 2)
        {
            encryptedPassword.push_back(static_cast<unsigned char>(std::stoi(encryptedPasswordHex.substr(i, 2), nullptr, 16)));
        }
        auto decryptedPassword = EncryptionNS::decrypt(encryptedPassword, PasswordManager::getEncryptionKey());

        EXPECT_EQ(decryptedPassword, maxPassword);
    }
    // Test case for handling missing credentials
    TEST(PasswordManagerTestSuite, GetNonExistentCredential)
    {
        PasswordManager pm;
        pm.setTestCredentials("testUser", "testPassword");

        // Attempting to get a credential that doesn't exist
        auto credential = pm.getCredential("nonExistentService");
        EXPECT_FALSE(credential.has_value());
    }
    TEST(PasswordManagerTestSuite, HasPasswordBranching)
{
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Branch where the password does not exist
    EXPECT_FALSE(pm.hasPassword("nonExistentService"));

    // Add a password and test the branch where the password exists
    pm.addNewPassword("email", "user@example.com", "password123");
    EXPECT_TRUE(pm.hasPassword("email"));
}

TEST(PasswordManagerTestSuite, RetrieveCredentialsBranching)
{
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Branch where credentials do not exist
    EXPECT_FALSE(pm.getCredential("nonExistent").has_value());

    // Branch where credentials exist
    pm.addNewPassword("email", "user@example.com", "password123");
    EXPECT_TRUE(pm.getCredential("email").has_value());
}
TEST(PasswordManagerTestSuite, HasPasswordCaseSensitivity)
{
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Branch where the password does not exist
    EXPECT_FALSE(pm.hasPassword("nonExistentService"));

    // Add a password and test the branch where the password exists
    pm.addNewPassword("email", "user@example.com", "password123");
    EXPECT_TRUE(pm.hasPassword("email"));

    // Test branch for case sensitivity (if applicable)
    EXPECT_FALSE(pm.hasPassword("Email")); // Assuming service names are case-sensitive
}
TEST(PasswordManagerTestSuite, HasPasswordBranchCoverage)
{
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Branch: Non-existent password
    EXPECT_FALSE(pm.hasPassword("nonExistentService"));

    // Branch: Existing password
    pm.addNewPassword("email", "user@example.com", "password123");
    EXPECT_TRUE(pm.hasPassword("email"));

    // Branch: Case sensitivity (if implemented)
    EXPECT_FALSE(pm.hasPassword("Email")); // Assuming service names are case-sensitive
}
TEST(PasswordManagerTestSuite, GetCredentialBranchCoverage)
{
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Branch: Credential does not exist
    EXPECT_FALSE(pm.getCredential("nonExistent").has_value());

    // Branch: Credential exists
    pm.addNewPassword("email", "user@example.com", "password123");
    EXPECT_TRUE(pm.getCredential("email").has_value());
}

TEST(PasswordManagerTestSuite, LoadCredentialsFileNotFound) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Ensure the file does not exist
    remove("user_credentials.csv");

    // Attempt to load credentials
    EXPECT_FALSE(pm.loadUserCredentialsFromFile());
}

TEST(PasswordManagerTestSuite, GeneratePasswordBranching) {
    PasswordManager pm;

    // Valid length
    EXPECT_NO_THROW(pm.generatePassword(12));

    // Invalid length
    EXPECT_THROW(pm.generatePassword(0), std::invalid_argument);
}

TEST(PasswordManagerBranchTestSuite, GeneratePassword_ValidLength) {
    PasswordManager pm;
    auto password = pm.generatePassword(12);
    EXPECT_EQ(password.length(), 12);
}
TEST(PasswordManagerBranchTestSuite, GetCredential_ExistingService) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    pm.addNewPassword("service4", "username4", "ValidPassword123");
    auto credential = pm.getCredential("service4");
    EXPECT_TRUE(credential.has_value());
}
// Test: Branch coverage for `addNewPassword`
TEST(PasswordManagerBranchCoverage, AddNewPasswordBranches) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Branch: Weak password
    EXPECT_THROW(pm.addNewPassword("service", "username", "short"), std::invalid_argument);

    // Branch: Valid password
    EXPECT_NO_THROW(pm.addNewPassword("email", "user@example.com", "StrongPassword123!"));
    EXPECT_TRUE(pm.hasPassword("email"));
}


// Test: Load Credentials From File Branches
TEST(PasswordManagerBranchCoverage, LoadCredentialsFromFileBranches) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Branch: File does not exist
    std::remove("testUser_passwords.dat"); // Ensure file does not exist
    EXPECT_THROW(pm.loadCredentialsFromFile(), std::ios_base::failure);

    // Branch: File exists
    std::ofstream file("testUser_passwords.dat");
    file << "service username:encryptedPassword\n";
    file.close();
    EXPECT_NO_THROW(pm.loadCredentialsFromFile());

    // Cleanup
    std::remove("testUser_passwords.dat");
}

// Test: Validate Branches
TEST(PasswordManagerBranchCoverage, ValidateBranches) {
    PasswordManager pm;

    // Branch: Weak password
    EXPECT_FALSE(pm.validate("short"));

    // Branch: Strong password
    EXPECT_TRUE(pm.validate("StrongPassword123!"));
}
// Test: Load User Credentials Branches
TEST(PasswordManagerBranchCoverage, LoadUserCredentialsBranches) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Branch: File does not exist
    std::remove("user_credentials.csv"); // Ensure file does not exist
    EXPECT_FALSE(pm.loadUserCredentialsFromFile());

    // Branch: File exists and credentials match
    std::ofstream file("user_credentials.csv");
    file << "testUser,testPassword\n";
    file.close();
    EXPECT_TRUE(pm.loadUserCredentialsFromFile());

    // Branch: File exists but credentials do not match
    pm.setTestCredentials("testUser", "wrongPassword");
    EXPECT_FALSE(pm.loadUserCredentialsFromFile());
}
// Test: Generate Password Branches
TEST(PasswordManagerBranchCoverage, GeneratePasswordBranches) {
    PasswordManager pm;

    // Branch: Valid length
    EXPECT_NO_THROW(pm.generatePassword(10));

    // Branch: Invalid length
    EXPECT_THROW(pm.generatePassword(0), std::invalid_argument);
}
// Test: Has Password Branches
TEST(PasswordManagerBranchCoverage, HasPasswordBranches) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");

    // Branch: Password does not exist
    EXPECT_FALSE(pm.hasPassword("nonExistentService"));

    // Branch: Password exists
    pm.addNewPassword("service", "user", "ValidPassword123");
    EXPECT_TRUE(pm.hasPassword("service"));
}

} // namespace
