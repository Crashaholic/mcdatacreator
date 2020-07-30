#pragma once

#include <imgui/imgui.h>
#include <imgui/include/imgui_impl_opengl3.h>
#include <imgui/include/imgui_impl_glfw.h>
#include "imgui_internal.h"
#include <imgui/misc/cpp/imgui_stdlib.h>

namespace WindowElements
{
	namespace Dockable
	{
		bool Begin(const char* label, bool* open = (bool*)0, ImVec2 MinWindowSize = ImVec2(300, 300), ImGuiWindowFlags additional_user_flags = (ImGuiWindowFlags)0)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, MinWindowSize);
			return ImGui::Begin(label, open, additional_user_flags);
		}

		void End()
		{
			ImGui::End();
			ImGui::PopStyleVar();
		}
	}
}
