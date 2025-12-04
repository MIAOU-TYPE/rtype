cmake_minimum_required(VERSION 3.20)
project(Buffer LANGUAGES CXX)

add_library(Buffer INTERFACE)

target_include_directories(Buffer
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}
)
