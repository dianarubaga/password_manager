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


} // namespace
