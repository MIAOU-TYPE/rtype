cmake_minimum_required(VERSION 3.20)
project(DLLoader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(DLLoaderLib SHARED
    src/DLLoader.cpp
    src/DLLoader.hpp
    src/DLLoader.tpp
)

target_include_directories(DLLoaderLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(Library::DLLoader ALIAS DLLoaderLib)

set_target_properties(DLLoaderLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
