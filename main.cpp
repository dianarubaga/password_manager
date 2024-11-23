#include "manager.h"
#include <iostream>
#include <limits>

using namespace PasswordNS;

int main() {
    PasswordManager pm;

    // Display the welcome message and options
    std::cout << "Welcome to Password Manager\n";
    std::cout << "1. Login\n";
    std::cout << "2. Create new account\n";
    std::cout << "3. Exit\n";
    
    int option;
    std::cin >> option;

    // Clear input buffer
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (option == 1) {  // Old user login
        std::string username, mainPassword;
        std::cout << "Enter username: ";
        std::getline(std::cin, username);
        std::cout << "Enter main password: ";
        std::getline(std::cin, mainPassword);

        pm.setTestCredentials(username, mainPassword);  // Setting credentials for login

        if (pm.loadUserCredentialsFromFile()) {
            std::cout << "Login successful. Loading your passwords...\n";
            pm.loadCredentialsFromFile();  // Load user-specific credentials
        } else {
            std::cout << "Invalid username or password.\n";
            return 1;
        }
    } else if (option == 2) {  // New user registration
        std::string username, mainPassword;
        std::cout << "Create your username: ";
        std::getline(std::cin, username);
        std::cout << "Create your main password: ";
        std::getline(std::cin, mainPassword);

        pm.setTestCredentials(username, mainPassword);  // Set credentials for the new account

        if (pm.validate(mainPassword)) {
            pm.encrypt(mainPassword);
            pm.saveUserCredentialsToFile();
            std::cout << "Account successfully created! You can now use the password manager.\n";
        } else {
            std::cout << "Password is too weak! Please choose a stronger password.\n";
            return 1;
        }
    } else {
        std::cout << "Exiting the program...\n";
        return 0;
    }

    int choice;
    do {
        std::cout << "\nPassword Manager Menu\n";
        std::cout << "1. Add a New Password\n";
        std::cout << "2. Show All Passwords\n";
        std::cout << "3. Generate Password\n";
        std::cout << "4. Delete a Password\n";
        std::cout << "5. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {  // Add a new password
                std::string serviceName, serviceUsername, password;
                std::cout << "Enter the name for this password (e.g., email, bank): ";
                std::getline(std::cin, serviceName);
                std::cout << "Enter the username for this service: ";
                std::getline(std::cin, serviceUsername);
                std::cout << "Enter password: ";
                std::getline(std::cin, password);
                pm.addNewPassword(serviceName, serviceUsername, password);
                break;
            }
            case 2: {  // Show all stored passwords
                pm.showAllPasswords();
                break;
            }
            case 3: {  // Generate a password and use it for a new entry
                int length;
                std::cout << "Enter password length: ";
                std::cin >> length;

                // Clear input buffer
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string generatedPassword = pm.generatePassword(length);
                std::cout << "Generated Password: " << generatedPassword << '\n';

                pm.useGeneratedPasswordForNewEntry(generatedPassword);
                break;
            }
            case 4: {  // Delete a password by service name
                std::string serviceName;
                std::cout << "Enter the service name of the password you want to delete: ";
                std::getline(std::cin, serviceName);
                pm.deletePassword(serviceName);
                break;
            }
            case 5:
                std::cout << "Exiting the program...\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}
