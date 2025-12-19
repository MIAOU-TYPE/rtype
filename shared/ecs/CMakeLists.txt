cmake_minimum_required(VERSION 3.20)

project(Ecs LANGUAGES CXX)

set(ECS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE ECS_SOURCES CONFIGURE_DEPENDS
        "${ECS_SRC_DIR}/*.cpp"
)

file(GLOB_RECURSE ECS_HEADERS CONFIGURE_DEPENDS
        "${ECS_SRC_DIR}/*.hpp"
        "${ECS_SRC_DIR}/*.tpp"
)

add_library(Ecs STATIC
        ${ECS_SOURCES}
        ${ECS_HEADERS}
)

target_include_directories(Ecs
        PUBLIC
        ${ECS_SRC_DIR}
)
