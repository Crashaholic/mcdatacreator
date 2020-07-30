#pragma once

#include <imgui/imgui.h>
#include <imgui/include/imgui_impl_opengl3.h>
#include <imgui/include/imgui_impl_glfw.h>
#include "imgui_internal.h"
#include <imgui/misc/cpp/imgui_stdlib.h>

namespace WindowElements
{
	namespace Modal
	{
		void OpenPopup(std::string DisplayWindowName, std::string UniqueWindowID)
		{
			ImGui::OpenPopup((DisplayWindowName + "##" + UniqueWindowID).c_str());
		}

		bool DoWindow(std::string WindowName, std::string WindowID, bool* show = (bool*)0, ImVec2 MinSize = { 200, 200 })
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, MinSize);
			return ImGui::BeginPopupModal((WindowName + "##" + WindowID).c_str(), show, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
		}

		bool DismissButton(std::string ButtonContent, std::string WindowID, bool* ToTurnOff)
		{
			if (ImGui::Button((ButtonContent + "##" + WindowID).c_str()))
			{
				ImGui::CloseCurrentPopup();
				*ToTurnOff = false;
				return true;
			}
			return false;
		}

		//inline void End() { ImGui::EndPopup(); }
	}
}