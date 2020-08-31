cmake_minimum_required(VERSION 3.17)
project(Luminous)

set(CMAKE_CXX_STANDARD 17)

add_executable(Luminous src/main.cpp src/parser/scene_parser.cpp)

file(GLOB_RECURSE ALL_FILES "src/*.h*" "src/*.c*")
foreach (_file IN ITEMS ${MORE_FILES})
    list(APPEND ALL_FILES ${_file})
endforeach ()

include_directories("${PROJECT_SOURCE_DIR}/src")