# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project(password_manager)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
endif()

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

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Include OpenSSL directories (adjust the path if necessary)
include_directories(/opt/homebrew/opt/openssl/include)
link_directories(/opt/homebrew/opt/openssl/lib)

# Main executable for the password manager
add_executable(password_manager main.cpp manager.cpp encryption.cpp ui.cpp)
target_include_directories(password_manager PRIVATE ${PROJECT_SOURCE_DIR})

# Link wxWidgets, OpenSSL, and other libraries
target_link_libraries(password_manager PRIVATE ${wxWidgets_LIBRARIES} HuffmanLib pthread OpenSSL::SSL OpenSSL::Crypto)

# Enable testing
enable_testing()

# Add the test executable for performance metrics
add_executable(performance_metrics performance_metrics.cpp encryption.cpp)
target_include_directories(performance_metrics PRIVATE ${PROJECT_SOURCE_DIR})

# Link OpenSSL and other libraries for performance testing
target_link_libraries(performance_metrics PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Add the test executable for unit tests
add_executable(test_password_manager test_password_manager.cpp manager.cpp encryption.cpp)
target_include_directories(test_password_manager PRIVATE ${PROJECT_SOURCE_DIR})

# Link GoogleTest, HuffmanLib, OpenSSL, and other libraries for unit testing
target_link_libraries(test_password_manager PRIVATE HuffmanLib GTest::gtest_main pthread OpenSSL::SSL OpenSSL::Crypto)

# Discover and run tests
include(GoogleTest)
gtest_discover_tests(test_password_manager)