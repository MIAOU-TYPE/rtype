cmake_minimum_required(VERSION 3.20)

# ------------------------------
# Project definition
# ------------------------------
project(NetPluginLib LANGUAGES CXX)

# ------------------------------
# Sources
# ------------------------------
file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# ------------------------------
# Build as shared library
# ------------------------------
add_library(NetPluginLib SHARED ${SOURCES})

# ------------------------------
# Include directories
# ------------------------------
target_include_directories(NetPluginLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ------------------------------
# Compile options
# ------------------------------
target_compile_features(NetPluginLib PUBLIC cxx_std_20)

# ------------------------------
# Optional: set output directory
# ------------------------------
set_target_properties(NetPluginLib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../build"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../build"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../build"
)

# ------------------------------
# Platform-specific libraries
# ------------------------------
if(WIN32)
    target_link_libraries(NetPluginLib PRIVATE ws2_32)
endif()

set_target_properties(NetPluginLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
