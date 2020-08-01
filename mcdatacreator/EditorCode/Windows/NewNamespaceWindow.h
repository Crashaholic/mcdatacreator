#pragma once

#include "Builders/WindowElements.h"
//#include "Builders/Window_NoDock.h"
#include "Builders/Window_Modal.h"
#include "EditorWindow.h"

struct NewNamespaceWindow : EditorWindow
{
	REGISTER_EDITOR_WINDOW(NewNamespaceWindow, "New Namespace");
	std::string NewProjectNamespace;
	bool CreateNewNamespace = false;

	void Show()
	{
		WindowElements::Modal::OpenPopup(DisplayWindowName, UniqueWindowID);
		if (WindowElements::Modal::DoWindow(DisplayWindowName, UniqueWindowID, &ShowThis))
		{
			WindowElements::InputText("New Namespace", &NewProjectNamespace);
			if (ImGui::Button("Create!"))
			{
				CreateNewNamespace = true;
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}
};