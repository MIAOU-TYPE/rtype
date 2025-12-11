cmake_minimum_required(VERSION 3.20)

# -----------------------------------
# Project definition
# -----------------------------------
project(CommandBuffer LANGUAGES CXX)

# -----------------------------------
# Header-only library
# -----------------------------------
add_library(CommandBuffer INTERFACE)

# -----------------------------------
# Include directories
# -----------------------------------

target_include_directories(CommandBuffer
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)
