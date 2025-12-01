project(r-type_server LANGUAGES CXX)

# ------------------------------
# SOURCE & INCLUDE PATHS
# ------------------------------
set(SERVER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS
        "${SERVER_SRC_DIR}/*.cpp"
)

file(GLOB_RECURSE SERVER_INCLUDES LIST_DIRECTORIES true "${SERVER_SRC_DIR}/*")

# ------------------------------
# EXECUTABLE
# ------------------------------
add_executable(${PROJECT_NAME}
        ${SERVER_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${SERVER_SRC_DIR}
        ${SERVER_INCLUDES}
)

# ------------------------------
# PLATFORM-SPECIFIC LIBS
# ------------------------------
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

# ------------------------------
# OUTPUT LOCATION
# ------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# ------------------------------
# EXPORT VARIABLES FOR TESTS
# ------------------------------
set(SERVER_SOURCES ${SERVER_SOURCES} PARENT_SCOPE)
set(SERVER_SRC_DIR ${SERVER_SRC_DIR} PARENT_SCOPE)
set(SERVER_INCLUDES ${SERVER_INCLUDES} PARENT_SCOPE)

# ------------------------------
# TESTS
# ------------------------------
enable_testing()
add_subdirectory(tests)
