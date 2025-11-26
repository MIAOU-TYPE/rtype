# CMakeLists.txt for client unit tests using GoogleTest framework

# -------------------------
# Test source discovery
# -------------------------
file(GLOB CONFIGURE_DEPENDS TEST_SOURCES "*.cpp")

# -------------------------
# Client sources (exclude Main.cpp)
# -------------------------
file(GLOB_RECURSE CLIENT_SOURCES "${CMAKE_SOURCE_DIR}/client/src/*.cpp")
list(FILTER CLIENT_SOURCES EXCLUDE REGEX ".*/Main\\.cpp$")

# -------------------------
# Create test executable
# -------------------------
add_executable(client_unit_tests ${TEST_SOURCES} ${CLIENT_SOURCES})

# -------------------------
# Include directories
# -------------------------
set(CLIENT_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/client/src")
file(GLOB_RECURSE _client_include_dirs LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/client/src/*")
foreach(path IN LISTS _client_include_dirs)
    if(IS_DIRECTORY "${path}")
        list(APPEND CLIENT_INCLUDE_DIRS "${path}")
    endif()
endforeach()
list(REMOVE_DUPLICATES CLIENT_INCLUDE_DIRS)

target_include_directories(client_unit_tests PRIVATE ${CLIENT_INCLUDE_DIRS})

# -------------------------
# SFML dependency
# -------------------------
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(client_unit_tests PRIVATE sfml-graphics sfml-window sfml-system)

# -------------------------
# GoogleTest dependency
# -------------------------
find_package(GTest CONFIG REQUIRED)
target_link_libraries(client_unit_tests PRIVATE GTest::gtest GTest::gtest_main)

# -------------------------
# Platform-specific libraries
# -------------------------
if(WIN32)
    target_link_libraries(client_unit_tests PRIVATE ws2_32)
    target_compile_definitions(client_unit_tests PRIVATE
        _WIN32_WINNT=0x0601
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(client_unit_tests PRIVATE pthread)
endif()

# -------------------------
# C++ standard
# -------------------------
target_compile_features(client_unit_tests PRIVATE cxx_std_20)

# -------------------------
# Compiler warnings
# -------------------------
if (MSVC)
    target_compile_options(client_unit_tests PRIVATE /W4)
else()
    target_compile_options(client_unit_tests PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# -------------------------
# Enable CTest
# -------------------------
enable_testing()
add_test(NAME client_unit_tests COMMAND client_unit_tests)
