#include "manager.h"
#include <gtest/gtest.h>

namespace {

// Test case for adding a new password
TEST(PasswordManagerTestSuite, AddNewPassword) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");  // No login required
    pm.addNewPassword("email", "user@example.com", "password123");
    EXPECT_EQ(pm.getCredential("email"), "password123");
}

// Test case for deleting a password
TEST(PasswordManagerTestSuite, DeletePassword) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");  // No login required
    pm.addNewPassword("bank", "user1", "securePassword");
    pm.deletePassword("bank");

    // Verify deletion
    EXPECT_FALSE(pm.hasPassword("bank"));
}

// Test case for checking if a password can be added and retrieved after deletion
TEST(PasswordManagerTestSuite, AddAfterDelete) {  // Fixed typo here
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");  // No login required
    pm.addNewPassword("bank", "user1", "securePassword");
    pm.deletePassword("bank");
    pm.addNewPassword("bank", "user1", "newPassword123");

    EXPECT_EQ(pm.getCredential("bank"), "newPassword123");
}

}  // namespace
