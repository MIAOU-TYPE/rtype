cmake_minimum_required(VERSION 3.20)

# ------------------------------
# Project definition
# ------------------------------
project(NetWrapperLib LANGUAGES CXX)

# ------------------------------
# Sources
# ------------------------------
set(SOURCES
        src/NetWrapper.cpp
)

# ------------------------------
# Build as shared library
# ------------------------------
add_library(NetWrapperLib STATIC ${SOURCES})

# ------------------------------
# Include directories
# ------------------------------
target_include_directories(NetWrapperLib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ------------------------------
# C++ standard
# ------------------------------
target_compile_features(NetWrapperLib PUBLIC cxx_std_20)

# ------------------------------
# Platform-specific libs
# ------------------------------
if (WIN32)
    target_link_libraries(NetWrapperLib PRIVATE ws2_32)
endif()

# -------------------------------------------------------
# Dependency: DLLoader
# -------------------------------------------------------
if (NOT TARGET DLLoaderLib)
    add_subdirectory(
        ${CMAKE_SOURCE_DIR}/shared/DLLoader
        ${CMAKE_CURRENT_BINARY_DIR}/DLLoader_build
    )
endif()

target_link_libraries(NetWrapperLib PUBLIC DLLoaderLib)