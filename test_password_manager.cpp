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

} // namespace
