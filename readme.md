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
├── build/                     # Build directory (created after running cmake)
├── googletest/                # GoogleTest submodule directory
├── Huffman-Encoder/           # Huffman encoding module
├── CMakeLists.txt             # CMake configuration file
├── encryption.cpp             # Implementation of encryption-related functionality
├── encryption.h               # Declaration of encryption-related functionality
├── huffman_compression.cpp    # Implementation of Huffman compression
├── main.cpp                   # Contains the main program logic
├── manager.cpp                # Implementation of Manager and PasswordManager classes
├── manager.h                  # Declaration of Manager and PasswordManager classes
├── performance_metrics.cpp    # Measures and reports performance metrics
├── readme.md                  # Documentation for the project
├── test_password_manager.cpp  # Unit tests for the PasswordManager class
├── ui.cpp                     # Implementation of user interface functionality
├── ui.h                       # Declaration of user interface functionality
├── user_credentials.csv       # Stores user credentials (username and password)
└── user_passwords.dat         # Stores passwords for user 'user'
```

## Setup and Build Instructions

### Prerequisites

- **CMake** (version 3.10 or higher)
- **C++ compiler** (such as g++, clang++)

### Steps to Build and Run

1. **Clone or download the repository** to your local machine.


2. **Clone google test** with this specific command 
```bash
git clone https://github.com/google/googletest.git /yourPathinyour computer/ 
```

3. **Add Huffman Encoding** as a submodule to directory:
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

7.  **Run CMake to generate the necessary build files**:

```bash
cmake ..
```
8.  **Compile** the project using make:

```bash
make
```
9.  **Run the program**: After building, run the program by executing the following command:

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

## Setting Up code coverage after running tests 

### 1. Make the code coverage 

```bash
 make coverage  
```
### 2. Open the code coverage online 

```bash
open coverage_report/index.html
```

## Setting up the Memory scanning

### 2. Use the `leaks` Tool to Analyze Memory

Use the `pgrep` command to find the process ID of the `password_manager` and pass it to the `leaks` tool:

```bash
leaks $(pgrep password_manager)
```
This command will:
- Locate the password_manager process using pgrep.
- Run the leaks utility to analyze the memory usage of the process.

## Running Performance Metrics

### 1. Run all commands for CMake as mentioned above
```bash
mkdir build
cd build
cmake ..
make
```

### 2. Run command to print the performance metrics for all functions

```bash
./performance_metrics
```

This command will:
- Run a performance analysis on all the functionalities in the code, the following functionalities were chosen
    - Encryption
    - Decryption
    - Generating Password
    - Saving CSV File
    - Loading from CSV File
- The code will return the time taken for the function to run with different password lengths or differnt input sizes depending on what is being measured. 
- Throughput was measured for both encryption and decryption

## Others

Documentation for Huffman Compression can be found here: [![Huffman](https://img.shields.io/badge/Testing-Documentation-blue)](./huffman_compression.md)