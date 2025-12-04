cmake_minimum_required(VERSION 3.20)
project(DLLoader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Library_DLLoader SHARED
    src/DLLoader.cpp
    src/DLLoader.hpp
    src/DLLoader.tpp
)

target_include_directories(Library_DLLoader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(Library::DLLoader ALIAS Library_DLLoader)

set_target_properties(Library_DLLoader PROPERTIES POSITION_INDEPENDENT_CODE ON)
