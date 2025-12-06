cmake_minimum_required(VERSION 3.20)

# ------------------------------
# Project definition
# ------------------------------
project(NetPacketLib LANGUAGES CXX)

# ------------------------------
# Sources
# ------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# ------------------------------
# Build as shared library
# ------------------------------
add_library(NetPacketLib SHARED ${SOURCES})

# ------------------------------
# Include directories
# ------------------------------
target_include_directories(NetPacketLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/src/UDPPacket

)

# ------------------------------
# Compile options
# ------------------------------
target_compile_features(NetPacketLib PUBLIC cxx_std_20)

# ------------------------------
# Output directories
# ------------------------------
set_target_properties(NetPacketLib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    POSITION_INDEPENDENT_CODE ON
)
