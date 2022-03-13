cmake_minimum_required(VERSION 3.8)
project(pathfinder)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_executable(viewer ${SOURCE_FILES})
