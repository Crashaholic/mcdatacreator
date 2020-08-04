#include <GLFW/glfw3.h>
#include "Editor.hpp"
#include <iostream>
#include <fstream>

#include "Windows\NewProjectWindow.h"
#include "Windows\NewNamespaceWindow.h"
#include "Windows\OpenProjectWindow.h"

#include "Windows\MCFunctionWindow.h"
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

#define NORMAL_LOOKING_STRING lexically_normal().string()

void Editor::ScanDirectory(string s)
{
	filesys::path p = s;
	ScanDirectory(p);
}

void Editor::ScanDirectory(filesys::path p)
{
	using recur_dir_iter = filesys::recursive_directory_iterator;
	std::cout << "Directory contents of " << p.NORMAL_LOOKING_STRING << ": \n\n=====\n";
	size_t Namespaces = 0;
	size_t Functions = 0;
	std::cout << "in this scan, counted:\n";
	std::string tempCurrentNamespace = "";
	std::string tempCurrentNamespaceDir = "";
	for (const auto& dirEntry : recur_dir_iter(p))
	{
		std::cout << dirEntry.path().NORMAL_LOOKING_STRING << '\n';

		if (dirEntry.path().NORMAL_LOOKING_STRING.find(".mcfunction") != string::npos)
		{
			if (tempCurrentNamespace.empty())
				throw ("item is not in a namespace!");

			Functions++;
		}
		else if (filesys::is_directory(dirEntry.path()) && dirEntry.path().parent_path() == p)
		{
			tempCurrentNamespace = dirEntry.path().filename().string();
			tempCurrentNamespaceDir = dirEntry.path().string();
			Namespaces++;
		}

	}
	std::cout << Namespaces << " namespaces and \n";
	std::cout << Functions << " functions.\n";
	std::cout << "=====\n\nEnd of Directory contents. Items are added to the project accordingly\n";
}

void Editor::NewProjectCreate()
{
	auto newProjectData = winStack.get<NewProjectWindow>();

	std::string ProjectPath = filesys::path(newProjectData->NewProjectDirectory).NORMAL_LOOKING_STRING; // creating a string from a temporary path here for consistency sake
	std::string ProjectName = newProjectData->NewProjectName;
	std::string ProjectNamespace = newProjectData->NewProjectNamespace;

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

	//Validate some inputs first
	if (!ValidateNamespace(&newProjectData->NewProjectNamespace))
	{
		//ModalUnimplemented = true;
		return;
	}

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
	McMetaData["pack"]["description"] = newProjectData->NewProjectDescription + " --- Made with MCDataCreator.";
	
	int PackFormat = 5;
	// haha switch cases dont work with c++ strings
	if (newProjectData->NewProjectMCVersion == "1.16.1")
	{
		PackFormat = 5;
	}
	McMetaData["pack"]["pack_format"] = PackFormat;
	std::ofstream thisMeta(projectMCMeta, std::ios::out);
	thisMeta << std::setw(4) << McMetaData << '\n';
	thisMeta.close();
	
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

	currentDatapackProject.~MCDatapackProject();
	currentDatapackProject = MCDatapackProject();

	currentDatapackProject.ProjectName = ProjectName;
	currentDatapackProject.ProjectRootDirectory = ProjectPath;
	currentDatapackProject.ProjectMCVersion = newProjectData->NewProjectMCVersion;
	currentDatapackProject.ProjectMCDPCVersion = "WIP_0.0.1";
	currentDatapackProject.ProjectDescription = newProjectData->NewProjectDescription;
	currentDatapackProject.Namespaces.push_back("minecraft");
	currentDatapackProject.Namespaces.push_back(ProjectNamespace);

	currentDatapackProject.Files.push_back(ProjectName + "/data/" + ProjectNamespace + "/functions/" + "init.mcfunction");
	currentDatapackProject.Files.push_back(ProjectName + "/data/" + ProjectNamespace + "/functions/" + "main.mcfunction");

	currentDatapackProject.Functions.push_back(MCFunction(ProjectNamespace +":init.mcfunction"));
	currentDatapackProject.Functions.push_back(MCFunction(ProjectNamespace +":main.mcfunction"));

	Json McProjectData = currentDatapackProject;
	std::ofstream thisProject(projectProperties, std::ios::out);
	thisProject << std::setw(4) << McProjectData << '\n';
	thisProject.close();

	newProjectData->ShowThis = false;

	winStack.add<ToolbarWindow>();
	winStack.add<EditorProjectExplorer>()->currentDatapackProject = &currentDatapackProject;

	glfwSetWindowTitle(_currentWindow, ProjectName.c_str());
}

void Editor::OpenProject()
{
	auto dataWindow = winStack.get<OpenProjectWindow>();

	if (dataWindow->OpenDirectoryString.empty())
	{
		dataWindow->startShowModal_InvalidProjectDirectory = true;
		dataWindow->DoOpenDir = false;
		return;
	}

	if (filesys::exists(dataWindow->OpenDirectoryString))
	{
		// ATTEMPT OPEN DIRECTORY
		if (filesys::is_directory(dataWindow->OpenDirectoryString))
		{
			std::cout << "Is Directory!\n";
			if (filesys::exists(dataWindow->OpenDirectoryString + "/data") && filesys::exists(dataWindow->OpenDirectoryString + "/pack.mcmeta") && filesys::exists(dataWindow->OpenDirectoryString + "/data/minecraft"))
			{
				currentDatapackProject.~MCDatapackProject();
				currentDatapackProject = MCDatapackProject();
				std::cout << "Is Valid Directory!\n";
				// TODO: CREATE FILE WITH THE DIRECTORY \
				   SCAN THE DIRECTORY FOR ITS CONTENTS, AUTOMATICALLY CREATE BASED ON IT
				ScanDirectory(dataWindow->OpenDirectoryString + "/data");
				dataWindow->ShowThis = false;
			}
			else
			{
				dataWindow->startShowModal_InvalidProjectDirectory = true;
			}
		}
		else if (filesys::is_regular_file(dataWindow->OpenDirectoryString))
		{
			std::cout << "Is File!\n";

			filesys::path p = dataWindow->OpenDirectoryString;
			if (p.extension() == ".mcdpproj")
			{
				std::ifstream input;
				input.open(dataWindow->OpenDirectoryString);
				std::stringstream stream;
				stream << input.rdbuf();
				input.close();

				// check to see if input mcdpproj file is defective or not
				// you never know if the mcdpproj file could just contain absolutely nothing

				std::string dump = stream.str();
				if (dump.find("ProjectName") == string::npos || dump.find("ProjectMCVersion")  == string::npos || dump.find("ProjectMCDPCVersion") == string::npos || dump.find("ProjectRootDirectory") == string::npos)
				{
					std::cout << "bad mcdpproj file!\n";
					dataWindow->DoOpenDir = false;
				}
				else
				{
					Json j;
					stream >> j;

					// checks to see if the given file is valid
					if (j.contains("ProjectName") && j.contains("ProjectMCVersion") && j.contains("ProjectMCDPCVersion") && j.contains("ProjectRootDirectory"))
					{
						std::cout << "valid file!\n";

						// check to see if the given directory matches written directory
						// if it isnt, change the project's written directory to reflect its new location
						// otherwise leave it as is.
						string checkPath = j["ProjectRootDirectory"];
						filesys::path pc = checkPath;

						if (pc.NORMAL_LOOKING_STRING != p.parent_path().parent_path().NORMAL_LOOKING_STRING)
						{
							j["ProjectRootDirectory"] = p.parent_path().parent_path().NORMAL_LOOKING_STRING;
							std::ofstream out;
							out.open(dataWindow->OpenDirectoryString);
							out << j;
							out.close();
						}

						currentDatapackProject.~MCDatapackProject();
						currentDatapackProject = j;

						// TODO: SCAN THE DIRECTORY FOR ITS CONTENTS, AUTOMATICALLY CREATE BASED ON IT

						dataWindow->ShowThis = false;
					}
					else
					{
						std::cout << "invalid file!\n";
					}
				}
			}
			else
			{
				std::cout << "invalid file!\n";
			}
		}
	}
	else
	{
		dataWindow->startShowModal_InvalidProjectDirectory = true;
	}

	dataWindow->DoOpenDir = false;
}

void Editor::Update()
{

	Dockspace.Update();
	if (Messenger::eat("showProjectExplorer"))
	{
		winStack.get<EditorProjectExplorer>()->currentDatapackProject = &currentDatapackProject;
	}
	winStack.check();
	winStack.show();

	if (DockspaceMenu.appShouldClose)
	{
		glfwSetWindowShouldClose(_currentWindow, true);
	}

	if (Messenger::eat("DEBUG__SCAN_DIR"))
	{
		ScanDirectory(std::string("C:\\Users\\User\\AppData\\Roaming\\.minecraft\\saves\\Skyblock 4.08.1\\datapacks\\skyblockDatapack\\data"));
	}

	if (Messenger::eat("CreateNewProject"))
	{
		NewProjectCreate();
	}
	if (Messenger::eat("CreateNewNamespace"))
	{
		auto window = winStack.get<NewNamespaceWindow>();
		currentDatapackProject.Namespaces.push_back(window->NewProjectNamespace);
	}
	if (Messenger::back().first == "FocusWindow")
	{
		std::string sender = Messenger::back().second;
		Messenger::pop();
		std::cout << "manually eating \"FocusWindow\" from " << sender << ". Leftover size is now: " << Messenger::size() << '\n';
		auto a = winStack.get<ToolbarWindow>();
		if (a)
		{
			a->FocussedFile = sender;
			if (Messenger::back().first == "IsRawView")
			{
				a->RawView = true;
				Messenger::pop();
			}
			else if (Messenger::back().first == "IsNotRawView")
			{
				a->RawView = false;
				Messenger::pop();
			}
		}
	}
	if (Messenger::eat("CreateNewNamespace"))
	{
		auto window = winStack.get<NewNamespaceWindow>();
		currentDatapackProject.Namespaces.push_back(window->NewProjectNamespace);
	}

	if (Messenger::eat("OpenProjectDirectory"))
	{
		OpenProject();
	}

	if (Messenger::eat("SaveAll"))
	{
		// TODO: SAVE ALL FILES
	}

	if (Messenger::eat("OpenProjectExplorerFile"))
	{
		filesys::path p = winStack.get<EditorProjectExplorer>()->out_selectedFile;
		if (filesys::exists(p))
		{
			if (p.extension() == ".mcfunction")
			{
				bool hasFile = false;
				size_t index = 0;
				for (size_t i = 0; i < currentDatapackProject.Functions.size(); ++i)
				{
					if (currentDatapackProject.Functions[i].Name.find(p.filename().string()) != string::npos)
					{
						hasFile = true;
						index = i;
					}
				}
				if (hasFile)
				{
					auto a = winStack.add<MCFunctionWindow>(p.filename().lexically_normal().string());
					a->referenceToMCFunc = &currentDatapackProject.Functions[index];
					a->fileName = p.filename().lexically_normal().string();
					a->fileDir = winStack.get<EditorProjectExplorer>()->out_selectedFile;
				}
			}
		}
		else
		{
			std::cout << "referenced file does not exist :<\n";
		}
	}

	Messenger::leftovers();

}
