cmake_minimum_required(VERSION 3.12)
project(KeyValueStorage VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable for main application
add_executable(key_value_storage 
    src/main.cpp
    src/storage.cpp
    src/command_processor.cpp
)

# Find Google Test package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(storage_tests 
    tests/storage_tests.cpp
    src/storage.cpp
)

# Link test executable with Google Test
target_link_libraries(storage_tests 
    ${GTEST_LIBRARIES}
    pthread
)

# Enable testing
enable_testing()
add_test(NAME StorageTests COMMAND storage_tests)