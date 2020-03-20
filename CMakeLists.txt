cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(pacman)

find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})

add_executable(pacman src/main.cpp src/MazeUtils.cpp)
# add_executable(pacman src/main.cpp src/MazeUtils.cpp)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(pacman ${SDL2_LIBRARIES})



