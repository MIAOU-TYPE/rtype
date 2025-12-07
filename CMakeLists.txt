cmake_minimum_required(VERSION 3.20)

# =============================================
# VCPKG TOOLCHAIN (auto-fetch + integration)
# =============================================
include(FetchContent)

FetchContent_Declare(
        vcpkg
        GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
        GIT_TAG master
)
FetchContent_MakeAvailable(vcpkg)

set(CMAKE_TOOLCHAIN_FILE
        "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file"
)

# =============================================
# PROJECT CONFIG
# =============================================
project(R-Type LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (BUILD_TESTING)
    enable_testing()
endif()

# =============================================
# fix for macOS + Homebrew LLVM bug
# (__hash_memory missing symbol)
# Force linking against Homebrew libc++ instead of Apple SDK libc++
# =============================================
if(APPLE)
    find_package(LLVM QUIET CONFIG)
    if(LLVM_FOUND)
        link_directories("${LLVM_LIBRARY_DIR}/c++")
        set(LLVM_LIBCXX_DIR "${LLVM_LIBRARY_DIR}/c++" CACHE STRING "" FORCE)
    endif()
endif()


# =============================================
# SUBMODULES
# =============================================
#------------------------------
# Buffer Library
#------------------------------
add_subdirectory(
        ${CMAKE_SOURCE_DIR}/shared/Buffer
        ${CMAKE_BINARY_DIR}/buffer_build
)

#------------------------------
# NetWrapper Library (shared)
#------------------------------
add_subdirectory(
        ${CMAKE_SOURCE_DIR}/shared/NetWrapper/Plugin
        ${CMAKE_BINARY_DIR}/NetPluginLib_build
)

#------------------------------
# NetWrapper Wrapper
#------------------------------
add_subdirectory(
        ${CMAKE_SOURCE_DIR}/shared/NetWrapper/Wrapper
        ${CMAKE_BINARY_DIR}/NetWrapper_build
)

#------------------------------
# NetPacket Library
#------------------------------
add_subdirectory(
        ${CMAKE_SOURCE_DIR}/shared/NetPacket
        ${CMAKE_BINARY_DIR}/NetPacketLib_build
)

#------------------------------
# NetProtocol Library
#------------------------------
add_subdirectory(
        ${CMAKE_SOURCE_DIR}/shared/Network
        ${CMAKE_BINARY_DIR}/NetProtocolLib_build
)

add_subdirectory(server)
add_subdirectory(client)