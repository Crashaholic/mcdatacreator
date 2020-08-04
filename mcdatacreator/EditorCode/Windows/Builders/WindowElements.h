#pragma once

#include <imgui/imgui.h>
#include <imgui/include/imgui_impl_opengl3.h>
#include <imgui/include/imgui_impl_glfw.h>
#include "imgui_internal.h"
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <string>

namespace WindowElements
{
	namespace _configuration
	{
		float offsetRelativity = 4.f;
	}
	namespace __privateNameSpace
	{

	}

	void ComboBox(const char* label, std::string* out_string, const char* strings_array[], ImGuiComboFlags flags = 0)
	{
		float total_w = ImGui::GetWindowContentRegionWidth(); 
		ImGui::Text(label);
		ImGui::SameLine(total_w / _configuration::offsetRelativity);
		if (ImGui::BeginCombo((std::string("##") + std::string(label)).c_str(), out_string->c_str(), flags))
		{
			for (int i = 0; i < IM_ARRAYSIZE(strings_array); i++)
			{
				bool isSelected = *out_string == strings_array[i];
				if (ImGui::Selectable(strings_array[i]))
				{
					*out_string = strings_array[i];
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	void InputText(const char* label, std::string* string, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callbacks = (ImGuiInputTextCallback)0, void* user_data = (void*)0)
	{
		float total_w = ImGui::GetWindowContentRegionWidth(); 
		ImGui::Text(label);
		ImGui::SameLine(total_w / _configuration::offsetRelativity);
		//ImGui::SetNextItemWidth(total_w); 
		ImGui::InputText((std::string("##") + std::string(label)).c_str(), string, flags, callbacks, user_data);
	}
}
