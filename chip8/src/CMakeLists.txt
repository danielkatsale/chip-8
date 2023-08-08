cmake_minimum_required(VERSION 3.10)

project(chip8)

add_executable(chip8 main.cpp chip8.cpp gfx.cpp)
#list(APPEND CMAKE_PREFIX_PATH ~/projects/cpp/lib/SDL/build)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(chip8 ${SDL2_LIBRARIES})
