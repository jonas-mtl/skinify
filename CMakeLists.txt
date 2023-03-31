# Project
cmake_minimum_required(VERSION 3.8)
project("OpenGL-StarterKit")

set(CMAKE_CXX_STANDARD 20)

# Source
add_executable(${PROJECT_NAME} src/main.cpp)

# OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
     message(STATUS "OpenGL was not found!")
endif(NOT OPENGL_FOUND)

# GLFW
add_subdirectory(lib/glfw)
target_include_directories(${PROJECT_NAME} PUBLIC lib/glfw/include)

# GLEW
add_subdirectory(lib/glew)
target_include_directories(${PROJECT_NAME} PUBLIC lib/glew/include)


target_link_libraries(${PROJECT_NAME}
	libglew_static
	OpenGL::GL
	glfw
)