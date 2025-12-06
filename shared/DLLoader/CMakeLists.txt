cmake_minimum_required(VERSION 3.20)

# ------------------------------
# Project definition
# ------------------------------
project(DLLoader LANGUAGES CXX)

# ------------------------------
# Set C++ standard
# ------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------
# Build as shared library
# ------------------------------
add_library(DLLoaderLib SHARED
    src/DLLoader.cpp
    src/DLLoader.hpp
    src/DLLoader.tpp
)

# ------------------------------
# Include directories
# ------------------------------
target_include_directories(DLLoaderLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ------------------------------
# Compile options
# ------------------------------
add_library(Library::DLLoader ALIAS DLLoaderLib)

# ------------------------------
# Output directories
# ------------------------------
set_target_properties(DLLoaderLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
