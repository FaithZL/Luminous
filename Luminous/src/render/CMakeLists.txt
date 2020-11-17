
add_library(luminous-plugins INTERFACE)
function(luminous_render_add_plugin name)
    cmake_parse_arguments(PLUGIN "" "CATEGORY" "SOURCES" ${ARGN})
    add_library(luminous-${PLUGIN_CATEGORY}-${name} MODULE ${PLUGIN_SOURCES})
    target_link_libraries(luminous-${PLUGIN_CATEGORY}-${name} PRIVATE mimalloc_config luminous-render)
    add_dependencies(luminous-plugins luminous-${PLUGIN_CATEGORY}-${name})
    set_target_properties(luminous-${PLUGIN_CATEGORY}-${name} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin/plugins/"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin/plugins/")
endfunction()

add_subdirectory(include)
add_subdirectory(samplers)
add_subdirectory(transforms)
add_subdirectory(rasterizers)
add_subdirectory(textures)
add_subdirectory(cameras)
add_subdirectory(filters)
add_subdirectory(shapes)