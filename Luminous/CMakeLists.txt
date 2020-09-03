cmake_minimum_required(VERSION 3.17)
project(Luminous)

# 项目信息
set(APP_NAME Luminous)

set(CMAKE_CXX_STANDARD 17)

add_executable(Luminous src/main.cpp src/parser/scene_parser.cpp src/core/samplers/Sampler.cpp src/core/samplers/Sampler.h src/core/CObject.h src/util/string_print.h src/util/sha1.cpp src/util/sha1.h src/math/data_types.h src/math/scalar_types.h src/math/vector_types.h src/math/matrix_types.h)

file(GLOB_RECURSE ALL_FILES "src/*.h*" "src/*.c*")
foreach (_file IN ITEMS ${MORE_FILES})
    list(APPEND ALL_FILES ${_file})
endforeach ()

include_directories("${PROJECT_SOURCE_DIR}/src")