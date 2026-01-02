##
## EPITECH PROJECT, 2025
## R-Type
## File description:
## CMakeLists
##

cmake_minimum_required(VERSION 3.20)

# ------------------------------
# Project definition
# ------------------------------
project(NetProtocol LANGUAGES CXX)

# ------------------------------
# Build as interface library
# ------------------------------
add_library(NetProtocol INTERFACE)

# ------------------------------
# Include directories
# ------------------------------
target_include_directories(NetProtocol
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Utils
        ${CMAKE_CURRENT_SOURCE_DIR}/Data/UDP
)
