#pragma once

#include "WindowElements.h"
#include "Window_NoDock.h"
#include "../DockspaceMenu.h"

struct
{
	std::string NewProjectNamespace;

	bool CreateNewNamespace = false;

	void Show()
	{
		WindowElements::NoDock::Begin("New Namespace##NewNamespaceWindow", &DockspaceMenu.showNewProjectNamespaceWindow);
		{
			WindowElements::InputText("Project Namespace", &NewProjectNamespace);
			if (ImGui::Button("Create!"))
			{
				CreateNewNamespace = true;
				DockspaceMenu.showNewProjectNamespaceWindow = false;
			}
		}
		WindowElements::NoDock::End();
	}
} NewNamespaceWindow;