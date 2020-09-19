cmake_minimum_required(VERSION 3.17)
project(Luminous)

# 项目信息
set(APP_NAME Luminous)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(thirdparty/spdlog)
add_subdirectory(tests)

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/ext/stblib/stb")

file(GLOB_RECURSE ALL_FILES "src/*.h*" "src/*.c*")

# 定义按目录结构归类的方法
function(assign_source_group)
    foreach (_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else ()
            set(_source_rel "${_source}")
        endif ()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach ()
endfunction(assign_source_group)

foreach (_source IN ITEMS ${ALL_FILES})
    assign_source_group(${_source})
endforeach ()


include_directories("${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include")


add_executable(${APP_NAME} ${ALL_FILES})



