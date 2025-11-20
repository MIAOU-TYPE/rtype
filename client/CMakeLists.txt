cmake_minimum_required(VERSION 3.10)
project(r-type_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
if (NOT SRCS)
    message(FATAL_ERROR "No .cpp source files found under ${SRC_DIR}")
endif()

set(INCLUDE_DIRS "${SRC_DIR}")
file(GLOB_RECURSE _maybe_dirs LIST_DIRECTORIES true "${SRC_DIR}/*")
foreach(path IN LISTS _maybe_dirs)
    if (IS_DIRECTORY "${path}")
        list(APPEND INCLUDE_DIRS "${path}")
    endif()
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system network audio REQUIRED)
if (SFML_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        sfml-graphics 
        sfml-window 
        sfml-system 
        sfml-network 
        sfml-audio
    )
else()
    message(FATAL_ERROR "SFML not found. Please install SFML 2.5 or higher.")
endif()

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _WIN32_WINNT=0x0601
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
