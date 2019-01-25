cmake_minimum_required(VERSION 3.12)
project(01_hello_SDL)

set(CMAKE_CXX_STANDARD 14)
# Include our own modules
#
##include_directories(${SDL2_INCLUDE_DIRS})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package (SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR}) 
#${SDL2_IMAGE_INCLUDE_DIRS})


add_executable(01_hello_SDL 01_hello_SDL.cpp)
target_link_libraries(01_hello_SDL ${SDL2_LIBRARY})
       
