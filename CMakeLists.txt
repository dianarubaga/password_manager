# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(password_manager)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
endif()

# Find GoogleTest
find_package(GTest REQUIRED)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for the project
include_directories(${PROJECT_SOURCE_DIR})

# Add the Huffman library
add_library(HuffmanLib Huffman-Encoding/Huffman_C/huffman.cpp)
target_include_directories(HuffmanLib PUBLIC Huffman-Encoding/Huffman_C)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Include OpenSSL directories (adjust the path if necessary)
include_directories(/opt/homebrew/opt/openssl/include)
link_directories(/opt/homebrew/opt/openssl/lib)

# Main executable for the password manager
add_executable(password_manager src/main.cpp src/manager.cpp src/encryption.cpp src/ui.cpp)
target_include_directories(password_manager PRIVATE ${PROJECT_SOURCE_DIR})

# Link wxWidgets, OpenSSL, and other libraries
target_link_libraries(password_manager PRIVATE ${wxWidgets_LIBRARIES} HuffmanLib pthread OpenSSL::SSL OpenSSL::Crypto)

# Enable testing
enable_testing()

# Add the test executable for unit tests
add_executable(test_password_manager test/test_password_manager.cpp src/manager.cpp src/encryption.cpp)
target_include_directories(test_password_manager PRIVATE ${PROJECT_SOURCE_DIR})

# Link GoogleTest, HuffmanLib, OpenSSL, and other libraries for testing
target_link_libraries(test_password_manager PRIVATE GTest::GTest GTest::Main HuffmanLib pthread OpenSSL::SSL OpenSSL::Crypto)

# Manually add tests
add_test(NAME PasswordManagerTest COMMAND test_password_manager)