# ------------------------------
# COLLECT TEST SOURCES
# ------------------------------
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Remove client Main.cpp from unit tests
list(FILTER CLIENT_SOURCES EXCLUDE REGEX "Main\\.cpp$")

# ------------------------------
# TEST EXECUTABLE
# ------------------------------
set(PROJECT_NAME client_unit_tests)

add_executable(${PROJECT_NAME}
        ${TEST_SOURCES}
        ${CLIENT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CLIENT_INCLUDE_DIRS}
)

# ------------------------------
# LINK LIBRARIES
# ------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE Buffer)
target_link_libraries(${PROJECT_NAME} PRIVATE NetWrapperLib)
target_link_libraries(${PROJECT_NAME} PRIVATE NetPacketLib)
target_link_libraries(${PROJECT_NAME} PRIVATE NetProtocol)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/shared/NetPacket/src
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/shared/NetWrapper/Wrapper
)

# ------------------------------
# SFML DEPENDENCIES
# ------------------------------
find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
        SFML::Network
)

# ------------------------------
# GTEST
# ------------------------------
find_package(GTest CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
)

# ------------------------------
# PLATFORM-SPECIFIC LIBS
# ------------------------------
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
# WARNINGS (less strict for tests)
# ------------------------------
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# ------------------------------
# ENABLE TESTING
# ------------------------------
enable_testing()

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

