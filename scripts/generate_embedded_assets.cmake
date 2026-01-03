# CMake script to generate embedded assets
# Usage: cmake -P generate_embedded_assets.cmake

set(CLIENT_ASSETS_DIR "${CMAKE_CURRENT_LIST_DIR}/../client/assets")
set(EMBEDDED_RESOURCES_DIR "${CMAKE_CURRENT_LIST_DIR}/../build/client/embedded_resources")
set(PYTHON_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/bin2header.py")

file(MAKE_DIRECTORY "${EMBEDDED_RESOURCES_DIR}")

# Automatically find all assets
file(GLOB_RECURSE ASSET_FILES
    "${CLIENT_ASSETS_DIR}/sprites/*.png"
    "${CLIENT_ASSETS_DIR}/fonts/*.otf"
    "${CLIENT_ASSETS_DIR}/fonts/*.ttf"
    "${CLIENT_ASSETS_DIR}/sounds/*.wav"
    "${CLIENT_ASSETS_DIR}/sounds/*.flac"
    "${CLIENT_ASSETS_DIR}/levels/*/*.json"
)

foreach(asset_file ${ASSET_FILES})
    if(EXISTS "${asset_file}")
        get_filename_component(asset_name "${asset_file}" NAME)
        string(REPLACE "." "_" safe_name "${asset_name}")
        string(REPLACE "-" "_" safe_name "${safe_name}")

        set(output_file "${EMBEDDED_RESOURCES_DIR}/${safe_name}.cpp")

        message(STATUS "Generating ${safe_name}.cpp from ${asset_name}")
        execute_process(
            COMMAND python3 "${PYTHON_SCRIPT}" "${asset_file}" "${output_file}"
            RESULT_VARIABLE result
        )

        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Failed to generate ${output_file}")
        endif()
    else()
        message(WARNING "Asset file not found: ${asset_file}")
    endif()
endforeach()

message(STATUS "All embedded assets generated successfully")
