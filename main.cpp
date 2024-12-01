#include "manager.h"
#include <iostream>
#include <stdexcept>
#include <limits>
#include <filesystem> // For checking file existence

using namespace PasswordNS;

int main() {
    PasswordManager pm;

    // Main Menu
    std::cout << "Welcome to Password Manager\n";
    std::cout << "1. Login\n";
    std::cout << "2. Create new account\n";
    std::cout << "3. Exit\n";

    int option;

    // Error Handling: Validate input for main menu selection
    if (!(std::cin >> option)) {
        std::cin.clear(); // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        throw std::invalid_argument("Invalid input. Please enter a number (1-3).");
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear leftover input

    if (option == 1) { // Old user login
        std::string username, mainPassword;
        std::cout << "Enter username: ";
        std::getline(std::cin, username);
        std::cout << "Enter main password: ";
        std::getline(std::cin, mainPassword);

        pm.setTestCredentials(username, mainPassword);

        // Decompress database if compressed file exists
        if (std::filesystem::exists("user_credentials.huff")) {
            if (!pm.loadUserCredentialsFromFile()) {
                throw std::runtime_error("Failed to decompress or load user credentials.");
            }
        } else {
            if (!pm.loadUserCredentialsFromFile()) {
                throw std::runtime_error("Invalid username or password.");
            }
        }

        std::cout << "Login successful. Loading your passwords..." << std::endl;
        pm.loadCredentialsFromFile();

    } else if (option == 2) { // New user registration
        std::string username, mainPassword;
        std::cout << "Create your username: ";
        std::getline(std::cin, username);
        std::cout << "Create your main password: ";
        std::getline(std::cin, mainPassword);

        pm.setTestCredentials(username, mainPassword);

        // Error Handling: Validate password strength
        if (!pm.validate(mainPassword)) {
            throw std::invalid_argument("Password is too weak. Must be longer than 8 characters.");
        }

        pm.saveUserCredentialsToFile();
        std::cout << "Account successfully created! You can now use the password manager." << std::endl;

    } else if (option == 3) { // Exit program
        std::cout << "Exiting the program..." << std::endl;
        return 0;

    } else { // Error Handling: Out-of-range menu option
        throw std::out_of_range("Invalid choice. Please enter a number between 1 and 3.");
    }

    // Password Manager Menu
    int choice;
    do {
        std::cout << "\nPassword Manager Menu\n";
        std::cout << "1. Add a New Password\n";
        std::cout << "2. Show All Passwords\n";
        std::cout << "3. Generate Password\n";
        std::cout << "4. Delete a Password\n";
        std::cout << "5. Exit and Compress Database\n";
        std::cout << "Choose an option: ";

        if (!(std::cin >> choice)) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            throw std::invalid_argument("Invalid input. Please enter a number (1-5).");
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear leftover input

        switch (choice) {
            case 1: { // Add a New Password
                std::string serviceName, serviceUsername, password;
                std::cout << "Enter the name for this password (e.g., email, bank): ";
                std::getline(std::cin, serviceName);
                std::cout << "Enter the username for this service: ";
                std::getline(std::cin, serviceUsername);
                std::cout << "Enter password: (must be longer than 8 char for security) ";
                std::getline(std::cin, password);

                // Error Handling: Validate non-empty input
                if (serviceName.empty() || serviceUsername.empty() || password.empty()) {
                    throw std::invalid_argument("Service name, username, or password cannot be empty.");
                }

                pm.addNewPassword(serviceName, serviceUsername, password);
                break;
            }
            case 2: { // Show All Passwords
                pm.showAllPasswords();
                break;
            }
            case 3: { // Generate Password
                int length;
                std::cout << "Enter password length: ";

                if (!(std::cin >> length) || length <= 0) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                    throw std::invalid_argument("Invalid length. Please enter a positive number.");
                }

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear leftover input

                std::string generatedPassword = pm.generatePassword(length);
                std::cout << "Generated Password: " << generatedPassword << std::endl;

                pm.useGeneratedPasswordForNewEntry(generatedPassword);
                break;
            }
            case 4: { // Delete a Password
                std::string serviceName;
                std::cout << "Enter the service name of the password you want to delete: ";
                std::getline(std::cin, serviceName);

                // Error Handling: Validate non-empty input
                if (serviceName.empty()) {
                    throw std::invalid_argument("Service name cannot be empty.");
                }

                pm.deletePassword(serviceName);
                break;
            }
            case 5: // Exit
                std::cout << "Exiting and compressing database..." << std::endl;
                pm.handleExit(); // Compress the database
                break;

            default: // Error Handling: Out-of-range menu option
                throw std::out_of_range("Invalid option. Please enter a number between 1 and 5.");
        }
    } while (choice != 5);

    return 0;
}