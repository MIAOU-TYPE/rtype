project(r-type_server LANGUAGES CXX)

# ------------------------------
# SOURCE & INCLUDE PATHS
# ------------------------------
set(SERVER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS
        "${SERVER_SRC_DIR}/*.cpp"
)

file(GLOB_RECURSE SERVER_HEADERS CONFIGURE_DEPENDS
        "${SERVER_SRC_DIR}/*.hpp"
        "${SERVER_SRC_DIR}/*.tpp"
)

set(SERVER_INCLUDE_DIRS "${SERVER_SRC_DIR}")
foreach(header ${SERVER_HEADERS})
    get_filename_component(dir "${header}" DIRECTORY)
    list(APPEND SERVER_INCLUDE_DIRS "${dir}")
endforeach()
list(REMOVE_DUPLICATES SERVER_INCLUDE_DIRS)

# ------------------------------
# EXECUTABLE
# ------------------------------
add_executable(${PROJECT_NAME}
        ${SERVER_SOURCES}
)

# -----------------------------
# fix for macOS + Homebrew LLVM bug
# (__hash_memory missing symbol)
# Force linking against Homebrew libc++ instead of Apple SDK libc++
# -----------------------------
if (APPLE AND DEFINED LLVM_LIBCXX_DIR)
    target_link_directories(${PROJECT_NAME} PUBLIC ${LLVM_LIBCXX_DIR})
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
        ${SERVER_INCLUDE_DIRS}
)

# ------------------------------
# PLATFORM-SPECIFIC LIBS
# ------------------------------
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif ()

# ------------------------------
# WARNINGS
# ------------------------------
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else ()
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
endif ()

# ------------------------------
# DEPENDENCIES
# ------------------------------

# ------------------------------
# Buffer
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE Buffer)

# ------------------------------
# COMMAND BUFFER
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE CommandBuffer)

# ------------------------------
# NetWrapperLib
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE NetWrapperLib)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/shared/NetWrapper/Wrapper
)

# ------------------------------
# NetPacketLib
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE NetPacketLib)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/shared/NetPacket/src
)

# ------------------------------
# NetProtocol
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE NetProtocol)


# ------------------------------
# EXPORT VARIABLES FOR TESTS
# ------------------------------
set(SERVER_SOURCES ${SERVER_SOURCES} PARENT_SCOPE)
set(SERVER_SRC_DIR ${SERVER_SRC_DIR} PARENT_SCOPE)
set(SERVER_INCLUDE_DIRS ${SERVER_INCLUDE_DIRS} PARENT_SCOPE)

# ------------------------------
# TESTS
# ------------------------------
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests" AND BUILD_TESTING)
    add_subdirectory(tests)
endif ()