#pragma once

#include <string>
#include <filesystem>
//#include "Builders/Window_NoDock.h"
#include "Builders/Window_Modal.h"
#include "Builders/WindowElements.h"
#include "../DockspaceMenu.h"
#include "EditorWindow.h"

struct NewProjectWindow : public EditorWindow
{
	NewProjectWindow()
	{
		UniqueWindowID = "NewProjectWindow";
		DisplayWindowName = "New Project";
		ShowThis = true;
	}

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

		WindowElements::Modal::OpenPopup(UniqueWindowID, DisplayWindowName);
		if (WindowElements::Modal::DoWindow(UniqueWindowID, DisplayWindowName, &ShowThis, ImVec2{ 600, 180 }))
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
					ShowThis = false;
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();

		if (ModalEmptyProjectName)
		{
			ImGui::OpenPopup("Empty Project Name##EmptyProjectNameModal");
		}

		if (ImGui::BeginPopupModal("Empty Project Name##EmptyProjectNameModal", &ModalEmptyProjectName, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Project name was empty; Do give a project name.");
			if (ImGui::Button("OK##EmptyProjectNameModalOk"))
			{
				ImGui::CloseCurrentPopup();
				ModalEmptyProjectName = false;
			}
			ImGui::EndPopup();
		}


	}
} /*NewProjectWindow*/;
