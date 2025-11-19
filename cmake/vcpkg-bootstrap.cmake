# Download VCPKG if missing
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/vcpkg")
    message(STATUS "VCPKG not found — downloading...")
    execute_process(
        COMMAND git clone https://github.com/microsoft/vcpkg ${CMAKE_SOURCE_DIR}/vcpkg
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

# Bootstrap (build vcpkg executable)
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/vcpkg")
    message(STATUS "Bootstrapping VCPKG...")
    execute_process(
        COMMAND ./bootstrap-vcpkg.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vcpkg
    )
endif()

# Export toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
