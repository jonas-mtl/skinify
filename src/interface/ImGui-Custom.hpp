#pragma once

#include "imgui.h"

namespace ImGui
{
	IMGUI_API bool SliderIntCustom(const char* label, int* v, int v_min, int v_max, ImVec2 cursorPos);
	IMGUI_API bool SliderFloatCustom(const char* label, float* v, float v_min, float v_max, ImVec2 cursorPos);
	IMGUI_API void TextCustom(const char* fmt, ImVec2 cursorPos, ...);
}