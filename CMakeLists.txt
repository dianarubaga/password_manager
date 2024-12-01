# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project(password_manager)

# Add Google Test submodule
add_subdirectory(googletest)

# Include Google Test headers
include_directories(googletest/include)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for the project
include_directories(${PROJECT_SOURCE_DIR})

# Add the Huffman library
add_library(HuffmanLib Huffman-Encoding/Huffman_C/huffman.cpp)
target_include_directories(HuffmanLib PUBLIC Huffman-Encoding/Huffman_C)

<<<<<<< HEAD
# Add an executable with the above sources
add_executable(password_manager ${SOURCES})

# Set the directories that should be included in the build command for this target
target_include_directories(password_manager
    PRIVATE 
        ${PROJECT_SOURCE_DIR}
)

# Link libraries (add any additional libraries if needed)
target_link_libraries(password_manager 
    PRIVATE 
        pthread 
        OpenSSL::SSL 
        OpenSSL::Crypto
)
=======
# Main executable for the password manager
add_executable(password_manager main.cpp manager.cpp)
target_include_directories(password_manager PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(password_manager PRIVATE HuffmanLib pthread)
>>>>>>> origin/main

# Enable testing
enable_testing()

# Add the test executable
<<<<<<< HEAD
add_executable(test_password_manager 
    manager.cpp
    test_password_manager.cpp
)

# Link the Google Test libraries to your test executable
target_link_libraries(test_password_manager 
    PRIVATE 
        GTest::gtest_main 
        pthread 
        OpenSSL::SSL 
        OpenSSL::Crypto
)
=======
add_executable(test_password_manager manager.cpp test_password_manager.cpp)
target_link_libraries(test_password_manager PRIVATE HuffmanLib GTest::gtest_main pthread)
>>>>>>> origin/main

# Discover and run tests
include(GoogleTest)
gtest_discover_tests(test_password_manager)

# Find OpenSSL
find_package(OpenSSL REQUIRED)
