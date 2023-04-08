#pragma once

#include "imgui.h"

namespace ImGui
{
	IMGUI_API bool SliderIntCustom(const char* label, int* v, int v_min, int v_max, float cursorPosX);
	IMGUI_API bool SliderFloatCustom(const char* label, float* v, float v_min, float v_max, float cursorPosX);
	IMGUI_API void TextCustom(const char* fmt, float cursorPosX, ...);
}