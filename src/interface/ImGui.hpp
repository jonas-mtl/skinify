#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui.h"

#include "Ui.hpp"

namespace Interface
{
	static Image* bannerImage;

	bool Render();
	Image* getBannerImage();
	void glfw_error_callback(int error, const char* description);
}