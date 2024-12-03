# Password Manager

A simple Password Manager built with C++ that allows users to create, store, and manage passwords for various services securely. This project demonstrates the use of inheritance in C++ along with basic file operations for storing user credentials and passwords.

## Features

- **Login or Create a New Account**: Users can log in using a username and password or create a new account.
- **Add Passwords**: Users can store passwords for different services (e.g., email, bank) and have them saved in a `.csv` file.
- **Show All Passwords**: Display all stored passwords for the logged-in user.
- **Generate Passwords**: Generate a random password of a user-specified length.
- **Delete Passwords**: Users can delete passwords by service name.
- **File-based Storage**: User credentials and passwords are stored in CSV

## Project Structure

Only relevant files are shown below:

```graphql
password_manager/
├── CMakeLists.txt         # CMake configuration file
├── include/
│   ├── encryption.h       # Declaration of encryption functions
│   ├── manager.h          # Declaration of Manager and PasswordManager classes
│   └── ui.h               # Declaration of UI classes
├── src/
│   ├── main.cpp           # Contains the main program logic
│   ├── manager.cpp        # Implementation of Manager and PasswordManager classes
│   ├── encryption.cpp     # Implementation of encryption functions
│   └── ui.cpp             # Implementation of UI classes
├── test/
│   └── test_password_manager.cpp # Unit tests for PasswordManager
```

## Installation & Build Instructions

### Prerequisites

- **CMake** (version 3.10 or higher)
- **C++ compiler** (such as g++, clang++)

### Installation

1. **Clone the repository** to your local machine.

   ```bash
   git clone https://github.com/dianarubaga/password_manager.git && cd password_manager
   ```

2. **Initialize Huffman Encoding** submodule:

   ```bash
   git submodule update --init --recursive
   ```

3. **Install wxWidgets**:

   ```bash
   brew install wxwidgets
   ```

4. **Create a build directory**:

   ```bash
   mkdir build && cd build
   ```

5. **Run CMake** to generate the necessary build files:

   ```bash
   cmake ..
   ```

6. **Compile** the project:

   ```bash
   make
   ```

7. **Run** the executable:

   ```bash
   ./password_manager
   ```

## Testing

To test the project, run the following command from the `build` directory after building the project:

```bash
./test_password_manager
```

---

Documentation for Huffman Compression can be found here: [![Huffman](https://img.shields.io/badge/Testing-Documentation-blue)](./huffman_compression.md)
