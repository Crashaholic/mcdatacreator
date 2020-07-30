#pragma once

#include "Builders/Window_NoDock.h"
#include "Builders/WindowElements.h"
#include "EditorWindow.h"
#include "../DockspaceMenu.h"

struct OpenProjectWindow : EditorWindow
{
	OpenProjectWindow()
	{

	}
	std::string OpenDirectoryString = "";

	bool DoOpenDir = false;

	void Show()
	{
		DockspaceMenu.showOpenProjectWindow = false;
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