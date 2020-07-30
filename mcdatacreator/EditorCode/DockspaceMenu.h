#pragma once

#include <imgui/imgui.h>
#include <imgui/include/imgui_impl_opengl3.h>
#include <imgui/include/imgui_impl_glfw.h>
#include "imgui_internal.h"
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "Windows/ShowWindowBools.h"

struct
{
	bool showNewProjectWindow = false;
	bool showNewProjectNamespaceWindow = false;
	bool showNewMCFunctionWindow = false;
	bool showNewJSONWindow = false;
	bool showOpenProjectWindow = false;
	bool showAddFileWindow = false;
	bool appShouldClose = false;
	void Update()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				showNewProjectWindow = true;
			}
			if (ImGui::MenuItem("New Namespace"))
			{
				showNewProjectNamespaceWindow = true;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("New MCFunction File"))
			{
				showNewMCFunctionWindow = true;
			}
			if (ImGui::MenuItem("New JSON File"))
			{
				showNewJSONWindow = true;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Open Project"))
			{
				showOpenProjectWindow = true;
			}
			if (ImGui::MenuItem("Add File To Project"))
			{
				showAddFileWindow = true;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit"))
			{
				appShouldClose = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Project Explorer"))
			{
				ConstantBools.showProjectExplorer = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("DEBUG"))
		{
			//if (ImGui::MenuItem("Open main mcfunction"))
			//{
			//	CurrentMCFunctionWindow.showThis = true;
			//}
			ImGui::EndMenu();
		}
	}
} DockspaceMenu ;
