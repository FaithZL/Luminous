include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/ext/stblib/stb")

foreach (_source IN ITEMS ${ALL_FILES})
    assign_source_group(${_source})
endforeach ()



include_directories("${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include")
include_directories("${PROJECT_SOURCE_DIR}/thirdparty/cxxopts/include")
include_directories("${PROJECT_SOURCE_DIR}/thirdparty/assimp/include")

add_library(luminous-all INTERFACE)
target_link_libraries(luminous-all INTERFACE luminous-core luminous-compute luminous-ext luminous-util)

add_subdirectory(core)
add_subdirectory(compute)
add_subdirectory(apps)
add_subdirectory(tests)
add_subdirectory(backends)
add_subdirectory(ext)
add_subdirectory(render)
add_subdirectory(util)
