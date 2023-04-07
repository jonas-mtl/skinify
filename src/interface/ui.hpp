#pragma once

#include <filesystem>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "../core/image.hpp"
#include "../skinify.hpp"


namespace UI {
	inline ImGuiStyle* style;
	inline ImFont* bigFont;

	bool LoadImage(Image& imageSrc, GLuint* imageOut, uint16_t* widthOut, uint16_t* heightOut);
	void LoadFont();

	void LoadTheme();

	void Render(GLFWwindow* window);
};