#pragma once

#include <string>
#include <filesystem>
#include "Builders/Window_Modal.h"
#include "Builders/WindowElements.h"
#include "EditorWindow.h"

struct NewProjectWindow : public EditorWindow
{
	//NewProjectWindow()
	//{
	//	UniqueWindowID = "NewProjectWindow";
	//	DisplayWindowName = "New Project";
	//	ShowThis = true;
	//}

	REGISTER_EDITOR_WINDOW(NewProjectWindow, "New Project");

	std::string NewProjectName;
	std::string NewProjectNamespace;
	std::string NewProjectDirectory = std::filesystem::current_path().string();
	std::string NewProjectDescription;
	std::string NewProjectMCVersion = "Please select a value";

	bool DoCreateNewProject = false;
	bool DoResetDirectoryString = false;

	bool ModalEmptyProjectName = false;

	void Show() override
	{
		const char* versionStrings[] = {"1.16.1"};

		WindowElements::Modal::OpenPopup(DisplayWindowName, UniqueWindowID);
		if (WindowElements::Modal::DoWindow(DisplayWindowName, UniqueWindowID, &ShowThis, ImVec2{ 600, 180 }))
		{
			WindowElements::InputText("Project Name", &NewProjectName);
			WindowElements::InputText("Project Namespace", &NewProjectNamespace);
			ImGui::Separator();
			WindowElements::InputText("Project Path", &NewProjectDirectory);
			ImGui::Separator();
			WindowElements::InputText("Project Description", &NewProjectDescription);
			WindowElements::ComboBox("Minecraft Version", &NewProjectMCVersion, versionStrings);

			if (ImGui::Button("Create##NewProjectBtn"))
			{
				if (NewProjectName.empty())
				{
					ModalEmptyProjectName = true;
					ImGui::OpenPopup("Empty Project Name##EmptyProjectNameModal");
				}
				else if (NewProjectMCVersion == "Please select a value")
				{
					
				}
				else if (NewProjectDirectory.empty())
				{

				}
				else
				{
					ImGui::CloseCurrentPopup();
					DoCreateNewProject = true;
				}
			}

			if (ImGui::BeginPopupModal("Empty Project Name##EmptyProjectNameModal", &ModalEmptyProjectName, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
			{
				ImGui::Text("Project name was empty; Do give a project name.");
				if (ImGui::Button("OK##EmptyProjectNameModalOk"))
				{
					ImGui::CloseCurrentPopup();
					ModalEmptyProjectName = false;
				}
				ImGui::EndPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}
};
