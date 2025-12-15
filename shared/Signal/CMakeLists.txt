cmake_minimum_required(VERSION 3.20)

project(Signal LANGUAGES CXX)

# ==============================
# Library
# ==============================
add_library(Signal STATIC)

# ==============================
# Public headers (API)
# ==============================
target_include_directories(Signal
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/SignalHandler
)

# ==============================
# Common sources
# ==============================
target_sources(Signal
        PRIVATE
        SignalHandler/SignalHandler.cpp
)

# ==============================
# Platform-specific sources
# ==============================
if (WIN32)
    target_sources(Signal
            PRIVATE
            WinHandler/WinHandler.cpp
    )
    target_compile_definitions(Signal PRIVATE SIGNAL_PLATFORM_WINDOWS)
else()
    target_sources(Signal
            PRIVATE
            PosixHandler/PosixHandler.cpp
    )
    target_compile_definitions(Signal PRIVATE SIGNAL_PLATFORM_POSIX)
endif()

# ==============================
# Compile options
# ==============================
target_compile_features(Signal PUBLIC cxx_std_20)
