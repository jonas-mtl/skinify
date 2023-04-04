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

#stb_image
target_include_directories(${PROJECT_NAME} PUBLIC lib/stb_image)
target_link_directories(${PROJECT_NAME} PUBLIC lib/stb_image)