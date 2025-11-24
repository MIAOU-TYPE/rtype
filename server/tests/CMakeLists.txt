# CMakeLists.txt for unit tests using GoogleTest framework
file(GLOB TEST_SOURCES "*.cpp")

# Recursively find all server sources (all subfolders)
file(GLOB_RECURSE SERVER_SOURCES "${CMAKE_SOURCE_DIR}/server/src/*.cpp")

# Exclude Main.cpp
list(FILTER SERVER_SOURCES EXCLUDE REGEX ".*/Main\\.cpp$")

# Create executable
add_executable(unit_tests ${TEST_SOURCES} ${SERVER_SOURCES})

# Include directories (all subfolders)
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
add_test(NAME unit_tests COMMAND unit_tests)
