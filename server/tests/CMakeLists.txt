file(GLOB TEST_SOURCES "*.cpp")
file(GLOB_RECURSE ECS_SOURCES "${CMAKE_SOURCE_DIR}/server/src/ecs/*.cpp")

add_executable(unit_tests ${TEST_SOURCES} ${ECS_SOURCES})

# Include directories
file(GLOB_RECURSE ALL_INCLUDE_DIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/server/src")
target_include_directories(unit_tests PRIVATE ${ALL_INCLUDE_DIRS})

# Link GoogleTest
find_package(GTest CONFIG REQUIRED)
target_link_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main)

# Platform-specific libs
if(UNIX AND NOT APPLE)
    target_link_libraries(unit_tests PRIVATE pthread)
endif()

# C++20
target_compile_features(unit_tests PRIVATE cxx_std_20)

# Enable CTest
enable_testing()

# Add test manually (most reliable method)
add_test(NAME unit_tests COMMAND unit_tests)