cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall -pthread")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}" -pthread)
set(CMAKE_BUILD_TYPE Debug) # TODO: Remove

project(pacman)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
message("Found package SDL2: ${SDL2_FOUND}")

find_package(SDL2_image REQUIRED)
message("Found package SDL2_image: ${SDL2_image_FOUND}")

add_executable(pacman src/main.cpp src/SDLWrapper.cpp src/Game.cpp src/Maze.cpp src/Agent.cpp src/Pacman.cpp src/Ghost.cpp)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
string(STRIP ${SDL2_IMAGE_LIBRARIES} SDL2_IMAGE_LIBRARIES)
target_link_libraries(pacman ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} -lpthread)



