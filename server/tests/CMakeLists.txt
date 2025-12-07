# ------------------------------
# COLLECT TEST SOURCES
# ------------------------------
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Remove server Main.cpp from unit tests
list(FILTER SERVER_SOURCES EXCLUDE REGEX "Main\\.cpp$")

# ------------------------------
# TEST EXECUTABLE
# ------------------------------
add_executable(unit_tests
        ${TEST_SOURCES}
        ${SERVER_SOURCES}
)

target_include_directories(unit_tests PRIVATE
        ${SERVER_INCLUDE_DIRS}
)

target_link_libraries(unit_tests PRIVATE Buffer)
target_link_libraries(unit_tests PRIVATE NetWrapperLib)
target_link_libraries(unit_tests PRIVATE NetPacketLib)
target_link_libraries(unit_tests PRIVATE NetPluginLib)
target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/shared/NetPacket/src
)
target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/shared/NetWrapper/Wrapper
)

# ------------------------------
# GTEST
# ------------------------------
find_package(GTest CONFIG REQUIRED)

target_link_libraries(unit_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
)

if (WIN32)
    target_link_libraries(unit_tests PRIVATE ws2_32)
endif()

# ------------------------------
# REGISTER TESTS
# ------------------------------
add_test(NAME unit_tests COMMAND unit_tests)
