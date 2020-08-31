cmake_minimum_required(VERSION 3.17)
project(Luminous)

set(CMAKE_CXX_STANDARD 17)

add_executable(Luminous src/main.cpp src/parser/scene_parser.cpp)

include_directories("${PROJECT_SOURCE_DIR}/src")