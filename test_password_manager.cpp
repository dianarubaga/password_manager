#include "manager.h"
#include <gtest/gtest.h>

using namespace PasswordNS; // This makes `PasswordManager` accessible without needing `PasswordNS::`

namespace {

// Test case for adding a new password
TEST(PasswordManagerTestSuite, AddNewPassword) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");  // No login required
    pm.addNewPassword("email", "user@example.com", "password123");
    EXPECT_EQ(pm.getCredential("email").value(), "password123"); // Use `.value()` with `std::optional`
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
TEST(PasswordManagerTestSuite, AddAfterDelete) {
    PasswordManager pm;
    pm.setTestCredentials("testUser", "testPassword");  // No login required
    pm.addNewPassword("bank", "user1", "securePassword");
    pm.deletePassword("bank");
    pm.addNewPassword("bank", "user1", "newPassword123");

    EXPECT_EQ(pm.getCredential("bank").value(), "newPassword123");
}

}  // namespace
