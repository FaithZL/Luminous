cmake_minimum_required(VERSION 3.17)
project(Luminous)

# 项目信息
set(APP_NAME Luminous)

set(CMAKE_CXX_STANDARD 17)

add_executable(Luminous src/main.cpp src/parser/scene_parser.cpp src/kernel/samplers/Sampler.cpp src/kernel/samplers/Sampler.h src/kernel/CObject.h src/tools/string_print.h)

file(GLOB_RECURSE ALL_FILES "src/*.h*" "src/*.c*")
foreach (_file IN ITEMS ${MORE_FILES})
    list(APPEND ALL_FILES ${_file})
endforeach ()

include_directories("${PROJECT_SOURCE_DIR}/src")