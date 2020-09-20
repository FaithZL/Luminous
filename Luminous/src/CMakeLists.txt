include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/ext/stblib/stb")

foreach (_source IN ITEMS ${ALL_FILES})
    assign_source_group(${_source})
endforeach ()

include_directories("${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include")

add_subdirectory(core)
add_subdirectory(compute)
add_subdirectory(apps)
add_subdirectory(tests)
