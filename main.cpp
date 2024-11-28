#include "manager.h"
#include <iostream>
#include <stdexcept>
#include <limits>

using namespace PasswordNS;

int main() {
    PasswordManager pm;

    // Main Menu
    try {
        std::cout << "Welcome to Password Manager\n";
        std::cout << "1. Login\n";
        std::cout << "2. Create new account\n";
        std::cout << "3. Exit\n";

        int option;
        std::cin >> option;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (option == 1) {
            std::string username, mainPassword;
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            std::cout << "Enter main password: ";
            std::getline(std::cin, mainPassword);

            pm.setTestCredentials(username, mainPassword);
            if (!pm.loadUserCredentialsFromFile()) {
                throw std::runtime_error("Invalid username or password.");
            }

            pm.loadCredentialsFromFile();
        } else if (option == 2) {
            std::string username, mainPassword;
            std::cout << "Create your username: ";
            std::getline(std::cin, username);
            std::cout << "Create your main password: ";
            std::getline(std::cin, mainPassword);

            pm.setTestCredentials(username, mainPassword);
            if (!pm.validate(mainPassword)) {
                throw std::invalid_argument("Password is too weak.");
            }

            pm.encrypt(mainPassword);
            pm.saveUserCredentialsToFile();
        } else if (option == 3) {
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

            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (choice) {
                case 1:
                    // Add new password
                    break;
                case 2:
                    pm.showAllPasswords();
                    break;
                case 5:
                    break;
                default:
                    std::cerr << "Invalid option." << std::endl;
            }
        } while (choice != 5);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}