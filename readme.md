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

```graphql
password_manager/
├── build/                 # Build directory (created after running cmake)
├── CMakeLists.txt         # CMake configuration file
├── main.cpp               # Contains the main program logic
├── manager.cpp            # Implementation of Manager and PasswordManager classes
├── manager.h              # Declaration of Manager and PasswordManager classes
├── user_credentials.csv   # Stores user credentials (username and password)
└── user_passwords.dat    # Stores passwords for user 'user'
```

## Setup and Build Instructions

### Prerequisites

- **CMake** (version 3.10 or higher)
- **C++ compiler** (such as g++, clang++)

### Steps to Build and Run

1. **Clone or download the repository** to your local machine.


2. **clone google test** with this specific command 
```bash
git clone https://github.com/google/googletest.git /yourPathinyour computer/ 
```

3.Add Huffman Encoding as a submodule:
   ```bash
   git submodule add https://github.com/BaptisteEtroy/Huffman-Encoding.git
   ```

4. **Navigate to the project directory**:

```bash
cd 
```

5. **Create a build directory**:

```bash
mkdir build
```

6. **Navigate to the build directory**:

```bash
cd build
```

7.  Run CMake to generate the necessary build files:

```bash
cmake ..
```
8.  Compile the project using make:

```bash
make
```
9.  Run the program: After building, run the program by executing the following command:

```bash
./password_manager
```

## Setting Up GoogleTest for Unit Testing

This project uses [GoogleTest](https://github.com/google/googletest) as the testing framework. Follow the instructions below to set it up in your environment:

### 1. Add GoogleTest as a Submodule

To include GoogleTest in your project, you need to add it as a submodule. Use the following command:

```bash
git submodule add https://github.com/google/googletest.git
```
### 2. Run the GoogleTest 

  Run the google test: after downloading googletest, run the test by executing the following command:

```bash
./test_password_manager
```

Documentation for Huffman Compression can be found here: [![Huffman](https://img.shields.io/badge/Testing-Documentation-blue)](./huffman_compression.md)