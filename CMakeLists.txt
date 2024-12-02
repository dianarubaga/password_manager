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

# Add coverage compile flags (only for GCC/Clang compilers)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage -g -O0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()


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

# Add test executable for Google Test
add_executable(test_password_manager test_password_manager.cpp manager.cpp encryption.cpp)
target_include_directories(test_password_manager PRIVATE ${PROJECT_SOURCE_DIR})

# Link Google Test, HuffmanLib, and OpenSSL libraries
target_link_libraries(test_password_manager PRIVATE gtest gtest_main HuffmanLib OpenSSL::SSL OpenSSL::Crypto pthread)

# Discover and run tests
include(GoogleTest)
gtest_discover_tests(test_password_manager)

# Add a custom target for code coverage
add_custom_target(coverage
    COMMAND lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1 --ignore-errors inconsistent
    COMMAND lcov --remove coverage.info '/usr/*' '*/googletest/*' '*/password_manager/Huffman-Encoding/Huffman_C/*' --output-file filtered_coverage.info --rc lcov_branch_coverage=1 --ignore-errors inconsistent
    COMMAND genhtml filtered_coverage.info --output-directory coverage_report --branch-coverage --ignore-errors inconsistent,category
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating code coverage report"
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Add tests to CTest
add_test(NAME PasswordManagerTests COMMAND test_password_manager)

# Add the performance metrics executable
add_executable(performance_metrics performance_metrics.cpp manager.cpp encryption.cpp)
target_include_directories(performance_metrics PRIVATE ${PROJECT_SOURCE_DIR})

# Link required libraries for performance metrics
target_link_libraries(performance_metrics PRIVATE HuffmanLib pthread OpenSSL::SSL OpenSSL::Crypto)

