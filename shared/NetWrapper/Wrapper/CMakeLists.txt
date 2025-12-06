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
add_library(NetWrapperLib SHARED ${SOURCES})

# ------------------------------
# Include directories
# ------------------------------
target_include_directories(NetWrapperLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/shared/DLLoader/src
)

# ------------------------------
# Compile options
# ------------------------------
target_compile_features(NetWrapperLib PUBLIC cxx_std_20)

# ------------------------------
# Platform-specific libraries
# ------------------------------
if(WIN32)
    target_link_libraries(NetWrapperLib PRIVATE ws2_32)
endif()

# ------------------------------
# Linker dependencies
# ------------------------------
# DLLoader est une librairie séparée que NetWrapper utilise
add_subdirectory(${CMAKE_SOURCE_DIR}/shared/DLLoader
                 ${CMAKE_BINARY_DIR}/DLLoader_build)
target_link_libraries(NetWrapperLib PRIVATE DLLoaderLib)

set_target_properties(NetWrapperLib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    POSITION_INDEPENDENT_CODE ON
)
