cmake_minimum_required(VERSION 3.20)

project(NetPacketLib LANGUAGES CXX)

# ------------------------------
# Sources
# ------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(NetPacketLib STATIC ${SOURCES})

# ------------------------------
# Include directories
# Make public the src/ folder
# ------------------------------
target_include_directories(NetPacketLib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/src/UDPPacket
        ${CMAKE_CURRENT_SOURCE_DIR}/src/TCPPacket
)

# ------------------------------
# C++20
# ------------------------------
target_compile_features(NetPacketLib PUBLIC cxx_std_20)

# ------------------------------
# Platform-specific networking
# ------------------------------
if(WIN32)
    target_link_libraries(NetPacketLib PRIVATE ws2_32)
endif()
