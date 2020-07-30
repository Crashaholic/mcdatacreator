#include <GLFW/glfw3.h>
#include "Editor.hpp"
#include <iostream>
#include <fstream>

#include "Windows\NewProjectWindow.h"
#include "Windows\NewNamespaceWindow.h"
#include "Windows\OpenProjectWindow.h"

#include "Windows\CurrentMCFunctionWindow.h"
#include "Windows\CurrentProjectExplorer.h"

#include "Dockspace.h"

void to_json(nlohmann::json& j, const MCDatapackProject& p) 
{
	j = nlohmann::json{
		{"Namespaces",				p.Namespaces},
		{"Files",					p.Files},
		{"ProjectMCDPCVersion",		p.ProjectMCDPCVersion},
		{"ProjectMCVersion",		p.ProjectMCVersion},
		{"ProjectRootDirectory",	p.ProjectRootDirectory},
		{"ProjectDescription",		p.ProjectDescription},
		{"ProjectName",				p.ProjectName},
	};
}

void from_json(const nlohmann::json& j, MCDatapackProject& p)
{
	//j.at("projectName").get_to(p.projectName);

	j.at("Namespaces")			.get_to(p.Namespaces);
	j.at("Files")				.get_to(p.Files);
	j.at("ProjectMCDPCVersion")	.get_to(p.ProjectMCDPCVersion);
	j.at("ProjectMCVersion")	.get_to(p.ProjectMCVersion);
	j.at("ProjectRootDirectory").get_to(p.ProjectRootDirectory);
	j.at("ProjectDescription")	.get_to(p.ProjectDescription);
	j.at("ProjectName")			.get_to(p.ProjectName);
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
			+> <ProjectName>.mcdpproj
			+> pack.mcmeta
	*/

	filesys::create_directory(ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions");


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
	McMetaData["pack"]["description"] = winStack.get<NewProjectWindow>()->NewProjectDescription + " --- Made with MCDataCreator.";
	
	int PackFormat = 5;
	// haha switch cases dont work with c++ strings
	if (winStack.get<NewProjectWindow>()->NewProjectMCVersion == "1.16.1")
	{
		PackFormat = 5;
	}
	McMetaData["pack"]["pack_format"] = PackFormat;
	std::ofstream thisMeta(projectMCMeta, std::ios::out);
	thisMeta << std::setw(4) << McMetaData << '\n';
	thisMeta.close();


	DockspaceMenu.showNewProjectWindow = false;
	currentDatapackProject = MCDatapackProject();

	currentDatapackProject.ProjectName = ProjectName;
	currentDatapackProject.ProjectRootDirectory = ProjectPath;
	currentDatapackProject.ProjectMCVersion = winStack.get<NewProjectWindow>()->NewProjectMCVersion;
	currentDatapackProject.ProjectMCDPCVersion = "WIP_0.0.1";
	currentDatapackProject.ProjectDescription = winStack.get<NewProjectWindow>()->NewProjectDescription;
	currentDatapackProject.Namespaces.push_back("minecraft");
	currentDatapackProject.Namespaces.push_back(ProjectNamespace);

	Json McProjectData = currentDatapackProject;
	std::ofstream thisProject(projectProperties, std::ios::out);
	thisProject << std::setw(4) << McProjectData << '\n';
	thisProject.close();
	
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

	winStack.get<NewProjectWindow>()->DoCreateNewProject = false;
	winStack.get<NewProjectWindow>()->NewProjectDirectory = filesys::current_path().string();

	CurrentMCFunctionWindow.showThis = true;
	CurrentMCFunctionWindow.fileDir = ProjectPath + "/" + ProjectName + "/data/" + ProjectNamespace + "/functions/" + "main.mcfunction";
	CurrentMCFunctionWindow.fileName = currentDatapackProject.Functions[1].Name;
	CurrentMCFunctionWindow.referenceToMCFunc = &currentDatapackProject.Functions[1];
}

void Editor::OpenProject()
{
	//if (OpenProjectWindow.OpenDirectoryString.empty())
	//{
	//	ModalInvalidProjectDirectory = true;
	//}
	//// ATTEMPT OPEN PROJECT
	//filesys::path currentPath = OpenProjectWindow.OpenDirectoryString;
	//if (filesys::is_directory(currentPath))
	//{
	//	//TODO: SUPPORT ADDING A DIRECTORY
	//	ModalInvalidProjectDirectory = true;
	//}
	//else
	//{
	//	Json data;
	//	std::ifstream inputFile = std::ifstream(OpenProjectWindow.OpenDirectoryString);
	//	inputFile >> data;
	//	if (data.contains("projectMCVersion"))
	//	{
	//		currentMCDPProjProp = data;
	//		if (!currentMCDPProjProp.CheckIntegrity())
	//		{
	//			ModalInvalidProjectDirectory = true;
	//		}
	//		else
	//		{
	//			currentDatapackProject.ProjectName = currentMCDPProjProp.projectName;
	//			currentDatapackProject.Namespaces = currentMCDPProjProp.Namespaces;
	//			currentDatapackProject.Files = currentMCDPProjProp.Files;
	//		}
	//	}
	//	else
	//		ModalInvalidProjectDirectory = true;
	//}
}

void Editor::Update()
{
	Dockspace.Update();
	winStack.check();
	winStack.show();

	//currentFileFullName = currentFileName + currentFileExtension;
	if (DockspaceMenu.appShouldClose)
	{
		glfwSetWindowShouldClose(_currentWindow, true);
	}

	if (DockspaceMenu.showNewProjectWindow)
	{
		//winStack.get<NewProjectWindow>()->ShowThis = true;
		winStack.add<NewProjectWindow>();
		DockspaceMenu.showNewProjectWindow = false;
	}

	if (winStack.get<NewProjectWindow>())
	{
		//winStack->get<NewProjectWindow>().Show();
		if (winStack.get<NewProjectWindow>()->DoCreateNewProject)
		{
			NewProjectCreate(winStack.get<NewProjectWindow>()->NewProjectName, winStack.get<NewProjectWindow>()->NewProjectNamespace, winStack.get<NewProjectWindow>()->NewProjectDirectory);
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

	if (ConstantBools.showProjectExplorer)
	{
		ConstantBools.showProjectExplorer = false;
		CurrentProjectExplorer.showThis = true;
		CurrentProjectExplorer.currentDatapackProject = &currentDatapackProject;
	}

	if (CurrentProjectExplorer.showThis)
	{
		CurrentProjectExplorer.Show();
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
