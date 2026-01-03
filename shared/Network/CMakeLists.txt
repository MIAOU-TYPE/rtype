##
## EPITECH PROJECT, 2026
## R-Type
## File description:
## CMakeLists
##

cmake_minimum_required(VERSION 3.20)

project(NetProtocol LANGUAGES CXX)

# ------------------------------
# Sources
# ------------------------------
set(SOURCES
        Data/TCP/payload/writer/TCPWriter.cpp
        Data/TCP/payload/reader/TCPReader.cpp
        Data/TCP/payload/TCPPayload.cpp
)

# ------------------------------
# Library
# ------------------------------
add_library(NetProtocol STATIC ${SOURCES})

# ------------------------------
# Include directories
# ------------------------------
target_include_directories(NetProtocol
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utils>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Data/TCP>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Data/TCP/payload>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Data/TCP/payload/writer>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Data/TCP/payload/reader>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Data/UDP>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# ------------------------------
# Compile options
# ------------------------------
target_compile_features(NetProtocol PUBLIC cxx_std_20)

# ------------------------------
# Platform-specific libraries
# ------------------------------
if(WIN32)
    target_link_libraries(NetProtocol PUBLIC ws2_32)
endif()
