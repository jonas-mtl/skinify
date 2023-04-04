# Project
cmake_minimum_required(VERSION 3.8)
project("Skinify")

set(CMAKE_CXX_STANDARD 20)

# Source
add_executable(${PROJECT_NAME} 
	src/main.cpp 
	src/image.cpp 
	src/skinify.cpp
)

# ImGui
target_include_directories(${PROJECT_NAME} PUBLIC lib/imgui)
target_link_directories(${PROJECT_NAME} PUBLIC lib/imgui)

# STB_image
target_include_directories(${PROJECT_NAME} PUBLIC lib/stb_image)
target_link_directories(${PROJECT_NAME} PUBLIC lib/stb_image)

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