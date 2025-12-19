cmake_minimum_required(VERSION 3.20)

# -----------------------------------
# Project definition
# -----------------------------------
project(Ecs LANGUAGES CXX)

# -----------------------------------
# Header-only library
# -----------------------------------
add_library(Ecs INTERFACE)

# -----------------------------------
# Include directories
# -----------------------------------

target_include_directories(Ecs
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
