#pragma once

#include <string>
#include <filesystem>
#include "Window_NoDock.h"
#include "WindowElements.h"
#include "../DockspaceMenu.h"

struct
{
	std::string NewProjectName;
	std::string NewProjectNamespace;
	std::string NewProjectDirectory = std::filesystem::current_path().string();
	std::string NewProjectDescription;
	std::string NewProjectMCVersion = "Please select a value";

	bool DoCreateNewProject = false;
	bool DoResetDirectoryString = false;

	bool ModalEmptyProjectName = false;

	void Show()
	{
		const char* versionStrings[] = {"1.16.1"};
		static const char* currentItem = NULL;
		WindowElements::NoDock::Begin("New Project##NewProjectWindow", &DockspaceMenu.showNewProjectWindow, ImVec2{ 600, 180 });
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
				else
				{
					DoCreateNewProject = true;
				}
			}
		}
		WindowElements::NoDock::End();
		if (ModalEmptyProjectName)
		{
			ImGui::OpenPopup("Empty Project Name##EmptyProjectNameModal");
		}

		if (ImGui::BeginPopupModal("Empty Project Name##EmptyProjectNameModal", &ModalEmptyProjectName, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Project name was empty; Do give a project name.");
			if (ImGui::Button("OK##EmptyProjectNameModalOk"))
			{
				//ErrorWindow.Close();
				ImGui::CloseCurrentPopup();
				ModalEmptyProjectName = false;
			}
			ImGui::EndPopup();
		}
	}
} NewProjectWindow;
