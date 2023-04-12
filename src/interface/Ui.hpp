#pragma once

#include <filesystem>
#include <GLFW/glfw3.h>
#include <cstring>
#include <imgui.h>

#include "../core/Image.hpp"
#include "../core/Skinify.hpp"
#include "ImGui-Custom.hpp"


namespace UI 
{
	inline ImGuiStyle* style;
	inline ImFont* bigFont;

	bool LoadImage(Image* imageSrc, GLuint* imageOut, uint16_t* widthOut, uint16_t* heightOut);
	void LoadFont();

	void LoadTheme();

	void Render(GLFWwindow* window);
};