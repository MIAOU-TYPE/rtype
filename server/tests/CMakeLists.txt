file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(FILTER SERVER_SOURCES EXCLUDE REGEX "Main\\.cpp$")

add_executable(unit_tests
        ${TEST_SOURCES}
        ${SERVER_SOURCES}
)

target_include_directories(unit_tests PRIVATE
        ${SERVER_SRC_DIR}
        ${SERVER_INCLUDES}
)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(unit_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
)

if (WIN32)
    target_link_libraries(unit_tests PRIVATE ws2_32)
endif()

add_test(NAME unit_tests COMMAND unit_tests)
