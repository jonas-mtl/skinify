#define IMGUI_DEFINE_MATH_OPERATORS

#include "ImGui-Custom.hpp"
#include "imgui.h"
#include "imgui_internal.h"


bool ImGui::SliderFloatCustom(const char* label, float* v, float v_min, float v_max, float cursorPosX)
{
	ImGui::SetCursorPosX(cursorPosX);
	return ImGui::SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max);
}

bool ImGui::SliderIntCustom(const char* label, int* v, int v_min, int v_max, float cursorPosX)
{
	ImGui::SetCursorPosX(cursorPosX);
	return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max);
}

void ImGui::TextCustom(const char* fmt, float cursorPosX, ...)
{
	va_list args;
	va_start(args, fmt);
	ImGui::SetCursorPosX(cursorPosX);
	ImGui::TextV(fmt, args);
	va_end(args);
}