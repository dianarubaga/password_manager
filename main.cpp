#include "manager.h"
#include <iostream>

int main() {
    PasswordManager pm;

    // Call login method to prompt user for login or account creation
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

        pm.setTestCredentials(username, mainPassword);  // Setting credentials for normal run

        if (pm.loadUserCredentialsFromFile()) {
            std::cout << "Login successful. Loading your passwords..." << std::endl;
            pm.loadCredentialsFromFile();  // Correct method to load service credentials after login
        } else {
            std::cout << "Invalid username or password." << std::endl;
            return 1;
        }
    } else if (option == 2) {  // New user registration
        std::string username, mainPassword;
        std::cout << "Create your username: ";
        std::getline(std::cin, username);
        std::cout << "Create your main password: ";
        std::getline(std::cin, mainPassword);

        pm.setTestCredentials(username, mainPassword);  // Setting credentials for new account

        if (pm.validate(mainPassword)) {
            pm.encrypt(mainPassword);
            pm.saveUserCredentialsToFile();
            std::cout << "Account successfully created! You can now use the password manager." << std::endl;
        } else {
            std::cout << "Password is too weak!" << std::endl;
            return 1;
        }
    } else {
        std::cout << "Exiting the program..." << std::endl;
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
            case 1: {
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
            case 2: {
                pm.showAllPasswords();
                break;
            }
            case 3: {
                int length;
                std::cout << "Enter password length: ";
                std::cin >> length;

                // Clear input buffer
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string generatedPassword = pm.generatePassword(length);
                std::cout << "Generated Password: " << generatedPassword << std::endl;

                pm.useGeneratedPasswordForNewEntry(generatedPassword);
                break;
            }
            case 4: {
                std::string serviceName;
                std::cout << "Enter the service name of the password you want to delete: ";
                std::getline(std::cin, serviceName);
                pm.deletePassword(serviceName);
                break;
            }
            case 5:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }

    } while (choice != 5);

    return 0;
}
