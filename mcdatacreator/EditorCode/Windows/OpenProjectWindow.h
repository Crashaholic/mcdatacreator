#pragma once

#include "Window_NoDock.h"
#include "WindowElements.h"
#include "../DockspaceMenu.h"

struct
{
	std::string OpenDirectoryString = "";

	bool DoOpenDir = false;

	void Show()
	{
		WindowElements::NoDock::Begin("Open Project##OpenProjectWindow", &DockspaceMenu.showOpenProjectWindow, {400, 100});
		{
			ImGui::Text("Insert a path to the .mcdpproj file");
			ImGui::Separator();
			WindowElements::InputText("Path", &OpenDirectoryString);
			if (ImGui::Button("Open##OpenProjectBtn"))
			{
				DoOpenDir = true;
				DockspaceMenu.showOpenProjectWindow = false;
			}
		}
		WindowElements::NoDock::End();
	}
} OpenProjectWindow;