project(r-type_client LANGUAGES CXX)

# ------------------------------
# SOURCE & INCLUDE PATHS
# ------------------------------
set(CLIENT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CLIENT_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(EMBEDDED_RESOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/embedded_resources")

file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS
        "${CLIENT_SRC_DIR}/*.cpp"
)

file(GLOB_RECURSE CLIENT_HEADERS CONFIGURE_DEPENDS
        "${CLIENT_SRC_DIR}/*.hpp"
        "${CLIENT_SRC_DIR}/*.tpp"
)

# ------------------------------
# EMBEDDED ASSETS GENERATION
# ------------------------------
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Generate embedded assets at configure time
message(STATUS "Generating embedded assets...")
execute_process(
    COMMAND "${CMAKE_COMMAND}" 
            -DCLIENT_ASSETS_DIR="${CLIENT_ASSETS_DIR}"
            -DEMBEDDED_RESOURCES_DIR="${EMBEDDED_RESOURCES_DIR}"
            -DPYTHON_SCRIPT="${CMAKE_SOURCE_DIR}/scripts/bin2header.py"
            -P "${CMAKE_SOURCE_DIR}/scripts/generate_embedded_assets.cmake"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE generation_result
)

if(NOT generation_result EQUAL 0)
    message(FATAL_ERROR "Failed to generate embedded assets")
endif()

# List generated files
file(GLOB EMBEDDED_SOURCES "${EMBEDDED_RESOURCES_DIR}/*.cpp")

# Add embedded sources to client sources
list(APPEND CLIENT_SOURCES ${EMBEDDED_SOURCES})

set(CLIENT_INCLUDE_DIRS "${CLIENT_SRC_DIR}")
foreach(header ${CLIENT_HEADERS})
    get_filename_component(dir "${header}" DIRECTORY)
    list(APPEND CLIENT_INCLUDE_DIRS "${dir}")
endforeach()
list(REMOVE_DUPLICATES CLIENT_INCLUDE_DIRS)

# ------------------------------
# SFML DEPENDENCIES
# ------------------------------
find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)

# ------------------------------
# EXECUTABLE
# ------------------------------
add_executable(${PROJECT_NAME}
        ${CLIENT_SOURCES}
)

# -----------------------------
# fix for macOS + Homebrew LLVM bug
# (__hash_memory missing symbol)
# Force linking against Homebrew libc++ instead of Apple SDK libc++
# -----------------------------
if (APPLE AND DEFINED LLVM_LIBCXX_DIR)
    target_link_directories(${PROJECT_NAME} PUBLIC ${LLVM_LIBCXX_DIR})
endif()


target_include_directories(${PROJECT_NAME} PRIVATE
        ${CLIENT_INCLUDE_DIRS}
)

# ------------------------------
# PLATFORM-SPECIFIC LIBS
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
        SFML::Network
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            ws2_32
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
            pthread
    )
endif()

# ------------------------------
# WARNINGS
# ------------------------------
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall -Wextra -Wpedantic
            -Wshadow -Wnon-virtual-dtor -Wold-style-cast
            -Wcast-align -Woverloaded-virtual -Wconversion
            -Wsign-conversion -Wnull-dereference -Wdouble-promotion
            -Wformat=2
    )
endif()

# ------------------------------
# DEPENDENCIES
# ------------------------------

# ------------------------------
# Buffer
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE Buffer)

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
# OUTPUT LOCATION
# ------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# ------------------------------
# EXPORT VARIABLES FOR TESTS
# ------------------------------
set(CLIENT_SOURCES ${CLIENT_SOURCES} PARENT_SCOPE)
set(CLIENT_SRC_DIR ${CLIENT_SRC_DIR} PARENT_SCOPE)
set(CLIENT_INCLUDE_DIRS ${CLIENT_INCLUDE_DIRS} PARENT_SCOPE)

# ------------------------------
# TESTS
# ------------------------------
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests" AND BUILD_TESTING)
    add_subdirectory(tests)
endif()