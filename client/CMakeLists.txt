cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Collect all source files
file(GLOB_RECURSE CLIENT_SOURCES "src/*.cpp")

add_executable(r-type_client ${CLIENT_SOURCES})

target_link_libraries(r-type_client sfml-graphics sfml-window sfml-system)

target_include_directories(r-type_client PRIVATE src)

# Place the executable at the root of the repo
set_target_properties(r-type_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})