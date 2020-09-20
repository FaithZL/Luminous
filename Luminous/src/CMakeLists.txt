

file(GLOB_RECURSE HEADER_FILES *.h*)
file(GLOB_RECURSE SOURCE_FILES *.c*)


include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/ext/stblib/stb")

foreach (_source IN ITEMS ${ALL_FILES})
    assign_source_group(${_source})
endforeach ()

include_directories("${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include")

add_subdirectory(core)
add_subdirectory(compute)



#add_executable(${APP_NAME} ${HEADER_FILES} ${SOURCE_FILES})