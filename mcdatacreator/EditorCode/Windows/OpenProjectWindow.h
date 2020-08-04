#pragma once

#include "Builders/Window_NoDock.h"
#include "Builders/WindowElements.h"
#include "EditorWindow.h"

struct OpenProjectWindow : EditorWindow
{
	REGISTER_EDITOR_WINDOW(OpenProjectWindow, "Open Project");

	std::string OpenDirectoryString = "";

	bool DoOpenDir = false;
	bool ModalInvalidProjectDirectory = false;
	bool startShowModal_InvalidProjectDirectory = false;

	void Show()
	{
		//DockspaceMenu.showOpenProjectWindow = false;
		WindowElements::Modal::OpenPopup(DisplayWindowName, UniqueWindowID);
		if (WindowElements::Modal::DoWindow(DisplayWindowName, UniqueWindowID, &ShowThis, {400, 100}))
		{
			ImGui::Text("Insert a path to the .mcdpproj file");
			ImGui::Separator();
			WindowElements::InputText("Path", &OpenDirectoryString);
			if (ImGui::Button("Open##OpenProjectBtn"))
			{
				Messenger::push("OpenProjectDirectory", UniqueWindowID);
				DoOpenDir = true;
			}

			if (startShowModal_InvalidProjectDirectory)
			{
				ImGui::OpenPopup("Invalid MCDPProject Directory##ModalInvalidProjectDirectory");
				startShowModal_InvalidProjectDirectory = false;
			}

			if (ImGui::BeginPopupModal("Invalid MCDPProject Directory##ModalInvalidProjectDirectory", &ModalInvalidProjectDirectory, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Given directory is not an MCDPProj file!");
				if (ImGui::Button("OK##ModalConfirm"))
				{
					ImGui::CloseCurrentPopup();
					ModalInvalidProjectDirectory = false;
				}
				ImGui::EndPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}
};