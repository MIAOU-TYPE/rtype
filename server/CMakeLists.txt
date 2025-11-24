project(r-type_server LANGUAGES CXX)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Woverloaded-virtual
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
    )
endif()

# -------------------------
# Source discovery
# -------------------------
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
if (NOT SRCS)
    message(FATAL_ERROR "No .cpp source files found under ${SRC_DIR}")
endif()

# -------------------------
# Include directories
# -------------------------
set(INCLUDE_DIRS "${SRC_DIR}")
file(GLOB_RECURSE SERVER_INCLUDE_DIRS LIST_DIRECTORIES true "${SRC_DIR}/*")
foreach(path ${SERVER_INCLUDE_DIRS})
    if(IS_DIRECTORY ${path})
        list(APPEND INCLUDE_DIRS ${path})
    endif()
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# -------------------------
# Main server executable
# -------------------------
add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# -------------------------
# Set output directory for the executable
# -------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

# -------------------------
# Compiler warnings
# -------------------------
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Woverloaded-virtual
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
    )
endif()

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _WIN32_WINNT=0x0601
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

#if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
#    add_subdirectory(tests)
#endif()

