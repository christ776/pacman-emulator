cmake_minimum_required(VERSION 3.13)
project(pacman_tile_parser)

set(CMAKE_CXX_STANDARD 14)

# Include our own modules
#
##include_directories(${SDL2_INCLUDE_DIRS})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package (SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR}) 
#${SDL2_IMAGE_INCLUDE_DIRS})


add_executable(pacman_tile_parser src/main.cpp 
                                src/parser/parser.cpp 
                                src/renderer/sdl_renderer.cpp
                                src/renderer/pixel.cpp)
target_link_libraries(pacman_tile_parser ${SDL2_LIBRARY})
       
