cmake_minimum_required(VERSION 3.13)
project(pacman_tile_parser)

set(CMAKE_CXX_STANDARD 14)

# Include our own modules
#
##include_directories(${SDL2_INCLUDE_DIRS})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenGL)
find_package(GLUT)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS}) 

add_executable(pacman_tile_parser src/main.cpp 
                                src/parser/parser.cpp 
                                src/renderer/opengl_renderer.cpp
                                src/renderer/pixel.cpp)
target_link_libraries(pacman_tile_parser ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
       
