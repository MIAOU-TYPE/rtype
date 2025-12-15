cmake_minimum_required(VERSION 3.20)

# ------------------------------
# Project definition
# ------------------------------
project(DLLoader LANGUAGES CXX)

# ------------------------------
# Build as static library
# ------------------------------
add_library(DLLoaderLib STATIC
        src/DLLoader.cpp
        src/DLLoader.hpp
        src/DLLoader.tpp
)

# ------------------------------
# Public include dirs
# ------------------------------
target_include_directories(DLLoaderLib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ------------------------------
# C++20 standard
# ------------------------------
target_compile_features(DLLoaderLib PUBLIC cxx_std_20)

# ------------------------------
# Platform-specific dependencies
# ------------------------------
if(WIN32)
    target_link_libraries(DLLoaderLib PRIVATE kernel32)
endif()

# ------------------------------
# Ensure PIC for shared lib
# ------------------------------
set_target_properties(DLLoaderLib PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

# ------------------------------
# Create an alias for clean linking
# ------------------------------
add_library(Library::DLLoader ALIAS DLLoaderLib)
