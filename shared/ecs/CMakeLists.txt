cmake_minimum_required(VERSION 3.20)

project(Ecs LANGUAGES CXX)

# ------------------------------
# Sources
# ------------------------------
set(ECS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB ECS_SOURCES CONFIGURE_DEPENDS
        "${ECS_SRC_DIR}/*.cpp"
)

file(GLOB ECS_HEADERS CONFIGURE_DEPENDS
        "${ECS_SRC_DIR}/*.hpp"
        "${ECS_SRC_DIR}/*.tpp"
)

# ------------------------------
# Library
# ------------------------------
add_library(Ecs STATIC
        ${ECS_SOURCES}
        ${ECS_HEADERS} # optionnel, mais pratique dans les IDE
)

# ------------------------------
# Includes
# ------------------------------
target_include_directories(Ecs
        PUBLIC
        ${ECS_SRC_DIR}
)

# ------------------------------
# Warnings (optionnel)
# ------------------------------
if (NOT MSVC)
    target_compile_options(Ecs PRIVATE
            -Wall -Wextra -Wpedantic
    )
endif()
