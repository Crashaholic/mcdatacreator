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
	bool appShouldClose = false;
	void Update()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				winStack.add<NewProjectWindow>();
			}
			if (ImGui::MenuItem("New Namespace"))
			{
				winStack.add<NewNamespaceWindow>();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("New MCFunction File"))
			{
			}
			if (ImGui::MenuItem("New JSON File"))
			{
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Open Project"))
			{
				winStack.add<OpenProjectWindow>();
			}
			if (ImGui::MenuItem("Add File To Project"))
			{
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
				Messenger::push("showProjectExplorer", "");
				winStack.add<EditorProjectExplorer>();
			}
			if (ImGui::MenuItem("Toolbar"))
			{
				winStack.add<ToolbarWindow>();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("DEBUG"))
		{
			//Messenger::push("token", "value", "sender");
			if (ImGui::MenuItem("DEBUG__SCAN_DIR"))
			{
				Messenger::push("DEBUG__SCAN_DIR", "");
			}
			ImGui::EndMenu();
		}
	}
} DockspaceMenu ;
