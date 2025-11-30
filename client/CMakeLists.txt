cmake_minimum_required(VERSION 3.20)
project(r-type_client LANGUAGES CXX)

# ------------------------------
# SOURCE & INCLUDE PATHS
# ------------------------------
set(CLIENT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS
        "${CLIENT_SRC_DIR}/*.cpp"
)

# Collect all include folders inside src/
file(GLOB_RECURSE CLIENT_INCLUDES LIST_DIRECTORIES true "${CLIENT_SRC_DIR}/*")

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

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CLIENT_SRC_DIR}
        ${CLIENT_INCLUDES}
)

# ------------------------------
# PLATFORM-SPECIFIC LIBRARIES
# ------------------------------
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network
            ws2_32
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network
            "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo"
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
            SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network
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
# OUTPUT LOCATION
# ------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# ------------------------------
# TESTS (optional)
# ------------------------------
if (BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()
