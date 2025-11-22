cmake_minimum_required(VERSION 3.20)

project(RType LANGUAGES CXX VERSION 1.0)

# Export compile commands for clang tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Language standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(server)
#add_subdirectory(client)

# Tests
if (EXISTS "${CMAKE_SOURCE_DIR}/tests")
    enable_testing()
    add_subdirectory(tests)
endif()
