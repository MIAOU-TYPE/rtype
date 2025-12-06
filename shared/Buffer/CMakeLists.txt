cmake_minimum_required(VERSION 3.20)

# ------------------------------
# Project definition
# ------------------------------
project(Buffer LANGUAGES CXX)

# ------------------------------
# Build as interface library
# ------------------------------
add_library(Buffer INTERFACE)

# ------------------------------
# Include directories
# ------------------------------
target_include_directories(Buffer
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
