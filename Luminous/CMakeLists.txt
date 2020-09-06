cmake_minimum_required(VERSION 3.17)
project(Luminous)

# 项目信息
set(APP_NAME Luminous)

set(CMAKE_CXX_STANDARD 17)

add_executable(Luminous src/main.cpp src/parser/scene_parser.cpp src/core/samplers/sampler.cpp src/core/samplers/sampler.h src/core/CObject.h src/util/string_print.h src/util/sha1.cpp src/util/sha1.h src/math/data_types.h src/math/scalar_types.h src/math/vector_types.h src/math/matrix_types.h src/math/math_util.h src/backends/cpu/cpu_device.cpp src/backends/cpu/cpu_device.h src/backends/cuda/cuda_device.cpp src/backends/metal/metal_device.mm src/compute/device.cpp src/compute/device.h src/util/plugin.h src/util/platform.h)

file(GLOB_RECURSE ALL_FILES "src/*.h*" "src/*.c*")
foreach (_file IN ITEMS ${MORE_FILES})
    list(APPEND ALL_FILES ${_file})
endforeach ()

include_directories("${PROJECT_SOURCE_DIR}/src")