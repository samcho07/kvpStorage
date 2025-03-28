cmake_minimum_required(VERSION 3.16)
project(kvpStorage 
    VERSION 1.0.0
    DESCRIPTION "Key-Value Pair Storage Application"
    LANGUAGES CXX
)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

# Require C++17 or later
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific configurations
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/storage.cpp
    src/command_processor.cpp
)

set(HEADERS
    src/storage.h
    src/command_processor.h
)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test executable
add_executable(${PROJECT_NAME}_tests 
    tests/storage_tests.cpp
    src/storage.cpp
    src/command_processor.cpp
)

# Link test executable with Google Test
target_link_libraries(${PROJECT_NAME}_tests 
    ${GTEST_LIBRARIES}
    pthread
)

# Add test to CTest
add_test(
    NAME ${PROJECT_NAME}_test
    COMMAND ${PROJECT_NAME}_tests
)

# Installation
install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)

# Packaging support
include(CPack)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "your.email@example.com")
