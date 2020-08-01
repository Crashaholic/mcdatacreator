#pragma once

#include <imgui/imgui.h>
#include <imgui/include/imgui_impl_opengl3.h>
#include <imgui/include/imgui_impl_glfw.h>
#include "imgui_internal.h"
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "Windows/ShowWindowBools.h"

#include "Windows/OpenProjectWindow.h"
#include "Windows/NewNamespaceWindow.h"
#include "Windows/CurrentProjectExplorer.h"
#include "Windows/NewProjectWindow.h"
#include "Windows/ToolbarWindow.h"
#include "WindowStack.h"

struct
{
	bool showNewProjectWindow = false;
	bool showNewProjectNamespaceWindow = false;
	bool showNewMCFunctionWindow = false;
	bool showNewJSONWindow = false;
	bool showOpenProjectWindow = false;
	bool showAddFileWindow = false;
	bool showProjectExplorer = false;
	bool appShouldClose = false;
	void Update()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				winStack.add<NewProjectWindow>();
				WindowElements::Modal::OpenPopup(winStack.get<NewProjectWindow>()->DisplayWindowName, winStack.get<NewProjectWindow>()->UniqueWindowID);
			}
			if (ImGui::MenuItem("New Namespace"))
			{
				winStack.add<NewNamespaceWindow>();
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
				winStack.add<OpenProjectWindow>();
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
				showProjectExplorer = true;
				winStack.add<EditorProjectExplorer>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("DEBUG"))
		{
			ImGui::EndMenu();
		}
	}
} DockspaceMenu ;
