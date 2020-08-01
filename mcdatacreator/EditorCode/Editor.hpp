#pragma once

#include <imgui/imgui.h>
#include <imgui/include/imgui_impl_opengl3.h>
#include <imgui/include/imgui_impl_glfw.h>
#include "imgui_internal.h"
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <string>
#include <filesystem>
#include <bitset>
#include "../MCDPProject/MCDatapackProject.hpp"


#include <json/json.hpp>

using Json = nlohmann::json;
using string = std::string;
namespace filesys = std::filesystem;

class Editor
{
public:
	enum fileType
	{
		json,
		mcfunction,
	};

	Editor(GLFWwindow* theWindow)
	{
		_currentWindow = theWindow;
	}

	string currentFileName;
	string currentFileExtension;
	string currentFileFullName;

	MCDatapackProject currentDatapackProject;

	// Creates the project files
	void NewProjectCreate();
	void OpenProject();
	void Update();

private:

	GLFWwindow* _currentWindow;
	bool ModalUnimplemented = false;
	//bool ModalInvalidProjectDirectory = false;
	bool ModalNamespaceHasUppercase = false;
};
