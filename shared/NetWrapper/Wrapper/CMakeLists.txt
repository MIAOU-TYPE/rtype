cmake_minimum_required(VERSION 3.20)
project(NetWrapperLib LANGUAGES CXX)

# ------------------------------
# Sources
# ------------------------------
set(SOURCES
    NetWrapper.cpp
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
        ${CMAKE_CURRENT_SOURCE_DIR}               # pour NetWrapper.hpp
        ${CMAKE_SOURCE_DIR}/shared/DLLoader/src   # pour DLLoader.hpp
)

# ------------------------------
# Compile options
# ------------------------------
target_compile_features(NetWrapperLib PUBLIC cxx_std_17)

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
target_link_libraries(NetWrapperLib PRIVATE Library_DLLoader)

set_target_properties(NetWrapperLib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../build"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../build"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../build"
    POSITION_INDEPENDENT_CODE ON
)
