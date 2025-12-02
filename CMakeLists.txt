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

enable_testing()

# =============================================
# LLVM FIX FOR macOS + HOMEBREW LLVM BUG
# (__hash_memory missing symbol)
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
add_subdirectory(server)
add_subdirectory(client)
