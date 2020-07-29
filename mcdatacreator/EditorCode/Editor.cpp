#include <GLFW/glfw3.h>
#include "Editor.hpp"
#include <iostream>
#include <fstream>

#include "Windows\NewProjectWindow.h"
#include "Windows\NewNamespaceWindow.h"
#include "Windows\OpenProjectWindow.h"

#include "Windows\CurrentMCFunctionWindow.h"

#include "Dockspace.h"

void to_json(nlohmann::json& j, const MCDPPRojProperties& p) 
{
	j = nlohmann::json{
		{"projectName", p.projectName},
		{"projectDirectory", p.projectDirectory},
		{"projectMCVersion", p.projectMCVersion},
		{"projectMCDPCVersion", p.projectMCDPCVersion},
		{"Namespaces", p.Namespaces},
		{"Files", p.Files},
		{"activeNamespaceIndex", p.activeNamespaceIndex},
		{"activeFileIndex", p.activeFileIndex}
	};
}

void from_json(const nlohmann::json& j, MCDPPRojProperties& p)
{
	j.at("projectName").get_to(p.projectName);
	j.at("projectDirectory").get_to(p.projectDirectory);
	j.at("projectMCVersion").get_to(p.projectMCVersion);
	j.at("projectMCDPCVersion").get_to(p.projectMCDPCVersion);
	j.at("Namespaces").get_to(p.Namespaces);
	j.at("Files").get_to(p.Files);
	j.at("activeNamespaceIndex").get_to(p.activeNamespaceIndex);
	j.at("activeFileIndex").get_to(p.activeFileIndex);
}

void Editor::NewProjectCreate(std::string ProjectName, std::string ProjectNamespace, std::string ProjectPath)
{
	/*
		Creates the directory:
		
		..
		=> <ProjectPath>
			+> <ProjectName>
				+> data
					+> minecraft
					+> <ProjectNameSpace>
	*/
	filesys::create_directory(ProjectPath + "/" + ProjectName);
	filesys::create_directory(ProjectPath + "/" + ProjectName + "/data");
	filesys::create_directory(ProjectPath + "/" + ProjectName + "/data/minecraft");
	filesys::create_directory(ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace);

	/*
		Creates the files:

		..
		=> <ProjectPath>
			+> <ProjectName>.mcdpproj
			+> pack.mcmeta
	*/
	filesys::path projectProperties = filesys::path(ProjectPath + "/" + ProjectName + "/" + ProjectName + ".mcdpproj");
	filesys::path projectMCMeta = filesys::path(ProjectPath + "/" + ProjectName + "/" + "pack" + ".mcmeta");
	/*
		File Contents
		[pack.mcmeta]
		{
			"pack": {
				"description": "<ProjectDescription> --- Made with MCDataCreator.",
				"pack_format": <ProjectPackFormat>
			}
		}

	*/
	// TODO: DO SOMETHING ABOUT PACK FORMAT
	Json McMetaData;
	McMetaData["pack"]["description"] = NewProjectWindow.NewProjectDescription + " --- Made with MCDataCreator.";
	
	int PackFormat = 0;
	// haha switch cases dont work with c++ strings
	if (NewProjectWindow.NewProjectMCVersion == "1.16.1")
	{
		PackFormat = 5;
	}
	McMetaData["pack"]["pack_format"] = PackFormat;
	std::ofstream thisMeta(projectMCMeta, std::ios::out);
	thisMeta << std::setw(4) << McMetaData << '\n';
	thisMeta.close();


	/*
		File Contents
		
		REFER TO MCDPProject/MCDPProjProperties

	*/

	currentMCDPProjProp = MCDPPRojProperties();
	currentMCDPProjProp.activeFileIndex = 0;
	currentMCDPProjProp.activeNamespaceIndex = 0;
	currentMCDPProjProp.Files = { ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions/" + "init.mcfunction"
		, ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions/" + "main.mcfunction" };
	currentMCDPProjProp.Namespaces = { "minecraft", ProjectNamespace };
	currentMCDPProjProp.projectDirectory = ProjectPath + '/' + ProjectName;
	currentMCDPProjProp.projectName = ProjectName;
	currentMCDPProjProp.projectMCDPCVersion = "PRE-ALPHA_1";
	currentMCDPProjProp.projectMCVersion = NewProjectWindow.NewProjectMCVersion;

	Json McProjectData = currentMCDPProjProp;
	std::ofstream thisProject(projectProperties, std::ios::out);
	//McProjectData["MCVersion"] = ProjectPath;
	//McProjectData << currentMCDPProjProp;
	thisProject << std::setw(4) << McProjectData << '\n';
	thisProject.close();



	DockspaceMenu.showNewProjectWindow = false;
	currentDatapackProject = MCDatapackProject();

	currentDatapackProject.ProjectName = ProjectName;
	currentDatapackProject.ProjectRootDirectory = ProjectPath;
	currentDatapackProject.ProjectDescription = NewProjectWindow.NewProjectDescription;
	currentDatapackProject.Namespaces.push_back("minecraft");
	currentDatapackProject.Namespaces.push_back(ProjectNamespace);
	


	/*
		Creates the directory:

		..
		=> <ProjectPath>
			=> <ProjectName>
				=> data
					=> minecraft
						+> tags
							+> functions
	*/
	filesys::create_directory(ProjectPath + "/" + ProjectName + "/data/minecraft/tags");
	filesys::create_directory(ProjectPath + "/" + ProjectName + "/data/minecraft/tags/functions");
	
	/*
		Creates the files:

		..
		=> <ProjectPath>
			=> data
				=> minecraft
					=> tags
						=> function
							+> load.json
							+> tick.json
	*/
	/*
		File Contents
		[load.json]
		
		{
			"values": [
				"<ProjectNamespace>:loop"
			]
		}
	*/
	std::ofstream thisLoad(ProjectPath + "/" + ProjectName + "/data/minecraft/tags/functions/" + "load.json", std::ios::out);
	Json LoadData;
	LoadData["values"] = { ProjectNamespace + ":init" };
	thisLoad << std::setw(4) << LoadData;
	thisLoad.close();

	/*
		File Contents
		[tick.json]

		{
			"values": [
				"<ProjectNamespace>:main"
			]
		}
	*/
	std::ofstream thisTick(ProjectPath + "/" + ProjectName + "/data/minecraft/tags/functions/" + "tick.json", std::ios::out);
	Json TickData;
	TickData["values"] = { ProjectNamespace + ":main" };
	thisTick << std::setw(4) << TickData;
	thisTick.close();

	filesys::create_directory(ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions");
	std::ofstream thisInit(ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions/" + "init.mcfunction", std::ios::out);
	thisInit << '\n';
	thisInit.close();
	std::ofstream thisMain(ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions/" + "main.mcfunction", std::ios::out);
	thisMain << '\n';
	thisMain.close();

	currentDatapackProject.Files.push_back(ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions/" + "init.mcfunction");
	currentDatapackProject.Files.push_back(ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions/" + "main.mcfunction");

	currentDatapackProject.Functions.push_back(MCFunction(ProjectNamespace +":init.mcfunction"));
	currentDatapackProject.Functions.push_back(MCFunction(ProjectNamespace +":main.mcfunction"));

	NewProjectWindow.DoCreateNewProject = false;
	NewProjectWindow.NewProjectDirectory = filesys::current_path().string();

	CurrentMCFunctionWindow.showThis = true;
	CurrentMCFunctionWindow.fileDir = ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions/" + "main.mcfunction";
	CurrentMCFunctionWindow.fileName = currentDatapackProject.Functions[1].Name;
	CurrentMCFunctionWindow.referenceToMCFunc = &currentDatapackProject.Functions[1];
}

void Editor::OpenProject()
{
	if (OpenProjectWindow.OpenDirectoryString.empty())
	{
		ModalInvalidProjectDirectory = true;
	}
	// ATTEMPT OPEN PROJECT
	filesys::path currentPath = OpenProjectWindow.OpenDirectoryString;
	if (filesys::is_directory(currentPath))
	{
		//TODO: SUPPORT ADDING A DIRECTORY
		ModalInvalidProjectDirectory = true;
	}
	else
	{
		Json data;
		std::ifstream inputFile = std::ifstream(OpenProjectWindow.OpenDirectoryString);
		inputFile >> data;
		if (data.contains("projectMCVersion"))
		{
			currentMCDPProjProp = data;
			if (!currentMCDPProjProp.CheckIntegrity())
			{
				ModalInvalidProjectDirectory = true;
			}
			else
			{
				currentDatapackProject.ProjectName = currentMCDPProjProp.projectName;
				currentDatapackProject.Files = currentMCDPProjProp.Files;
			}
		}
		else
			ModalInvalidProjectDirectory = true;
	}
}

void Editor::Update()
{
	Dockspace.Update();

	//currentFileFullName = currentFileName + currentFileExtension;
	if (DockspaceMenu.appShouldClose)
	{
		glfwSetWindowShouldClose(_currentWindow, true);
	}

	if (DockspaceMenu.showNewProjectWindow)
	{
		NewProjectWindow.Show();
		if (NewProjectWindow.DoCreateNewProject)
		{
			NewProjectCreate(NewProjectWindow.NewProjectName, NewProjectWindow.NewProjectNamespace, NewProjectWindow.NewProjectDirectory);
		}
	}

	if (DockspaceMenu.showNewProjectNamespaceWindow)
	{
		NewNamespaceWindow.Show();
		if (NewNamespaceWindow.CreateNewNamespace)
		{
			currentDatapackProject.Namespaces.push_back(NewNamespaceWindow.NewProjectNamespace);
		}
	}

	if (DockspaceMenu.showOpenProjectWindow)
	{
		OpenProjectWindow.Show();
		if (OpenProjectWindow.DoOpenDir)
		{
			OpenProject();
		}
	}

	if (CurrentMCFunctionWindow.showThis)
	{
		if (CurrentMCFunctionWindow.fileName.empty())
		{
			CurrentMCFunctionWindow.referenceToMCFunc = &currentDatapackProject.Functions[1];
			CurrentMCFunctionWindow.fileName = currentDatapackProject.Functions[1].Name;
			CurrentMCFunctionWindow.fileDir = currentDatapackProject.Files[1];
		}
		CurrentMCFunctionWindow.Show();
	}

#pragma region muh modals
	if (ModalUnimplemented)
	{
		ImGui::OpenPopup("Not Implemented##ModalNotImplemented");

		if (ImGui::BeginPopupModal("Not Implemented##ModalNotImplemented", &ModalUnimplemented, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Whatever you've done, that isn't implemented yet.");
			if (ImGui::Button("Aw RIP##ModalConfirm"))
			{
				ImGui::CloseCurrentPopup();
				ModalUnimplemented = false;
			}
			ImGui::EndPopup();
		}
	}

	if (ModalInvalidProjectDirectory)
	{
		ImGui::OpenPopup("Invalid MCDPProject Directory##ModalInvalidProjectDirectory");

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
	}
#pragma endregion
}
