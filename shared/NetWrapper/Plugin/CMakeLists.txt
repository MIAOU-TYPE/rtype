cmake_minimum_required(VERSION 3.20)

project(NetPluginLib LANGUAGES CXX)

# ------------------------------
# Sources
# ------------------------------
set(SOURCES
        src/NetPlugin.cpp
)

add_library(NetPluginLib SHARED ${SOURCES})

# ------------------------------
# Include directories
# ------------------------------
target_include_directories(NetPluginLib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# ------------------------------
# Compile options
# ------------------------------
target_compile_features(NetPluginLib PUBLIC cxx_std_20)

# ------------------------------
# Platform-specific libraries
# ------------------------------
if(WIN32)
    target_link_libraries(NetPluginLib PRIVATE ws2_32)
endif()
