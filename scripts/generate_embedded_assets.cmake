# CMake script to generate embedded assets
# Usage: cmake -P generate_embedded_assets.cmake

set(CLIENT_ASSETS_DIR "${CMAKE_CURRENT_LIST_DIR}/../client/assets")
set(EMBEDDED_RESOURCES_DIR "${CMAKE_CURRENT_LIST_DIR}/../build/client/embedded_resources")
set(PYTHON_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/bin2header.py")

file(MAKE_DIRECTORY "${EMBEDDED_RESOURCES_DIR}")

set(ASSET_FILES
    "${CLIENT_ASSETS_DIR}/sprites/bg-preview.png"
    "${CLIENT_ASSETS_DIR}/sprites/background_space.png"
    "${CLIENT_ASSETS_DIR}/sprites/play_bt.png"
    "${CLIENT_ASSETS_DIR}/sprites/play_bt_hold.png"
    "${CLIENT_ASSETS_DIR}/sprites/play_bt_press.png"
    "${CLIENT_ASSETS_DIR}/fonts/r-type.otf"
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
