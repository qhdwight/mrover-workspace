cmake_minimum_required(VERSION 3.8)
project(pathfinder)
set(CMAKE_CXX_STANDARD 17)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(rapidjson REQUIRED)
include_directories(${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/viewer"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/viewer/imgui"
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(
    ${PROJECT_NAME}
    ${CMAKE_DL_LIBS} ${SPECIAL_OS_LIBS}
    OpenGL::GL GLEW::glew glfw
)
