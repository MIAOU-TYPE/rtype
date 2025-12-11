# ------------------------------
# COLLECT TEST SOURCES
# ------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Remove server Main.cpp from unit tests
list(FILTER SERVER_SOURCES EXCLUDE REGEX "Main\\.cpp$")

# ------------------------------
# TEST EXECUTABLE
# ------------------------------
set(PROJECT_NAME unit_tests_shared)

add_executable(${PROJECT_NAME}
        ${TEST_SOURCES}
        ${SERVER_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
            ${SERVER_INCLUDE_DIRS}
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
# GTEST
# ------------------------------
find_package(GTest CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        GTest::gtest
        GTest::gtest_main
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

# ------------------------------
# REGISTER TESTS
# ------------------------------
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})


# ------------------------------
# SHARED LIBRARY FOR TESTING DLLOADER
# ------------------------------
add_library(testlib SHARED libs/testlib.cpp)

# Pour que DLLoader la trouve facilement
set_target_properties(testlib PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
