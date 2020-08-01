#pragma once

#include "Builders/Window_Dockable.h"
#include "Builders/WindowElements.h"
#include "../../MCDPProject/MCFunction.h"
#include "EditorWindow.h"

struct MCFunctionWindow : EditorWindow
{
	MCFunction* referenceToMCFunc = nullptr;
	bool EditRaw = false;

	std::string fileDir = "";
	std::string fileName = "";
	std::string MCVersion = "";
	
	MCFunctionWindow()
	{
		UniqueWindowID = "Undefined";
		DisplayWindowName = "Undefined";
		ShowThis = true;
	}

	void Show() override
	{
		if ((UniqueWindowID == std::string("Undefined") || UniqueWindowID == std::string("Undefined") && DisplayWindowName == std::string("Undefined")) && !fileName.empty())
		{
			UniqueWindowID = fileDir.c_str();
			DisplayWindowName = fileName.c_str();
			//ShowThis = false;
			//return;
		}
		else if (!referenceToMCFunc || fileName.empty())
		{
			ShowThis = false;
			return;
		}
		UniqueWindowID = DisplayWindowName = fileName.c_str();
		WindowElements::Dockable::Begin((fileName).c_str(), UniqueWindowID, &ShowThis);
		{
			std::cout << "SHOWING: " << UniqueWindowID << '\n';
			if (ImGui::Button("Add##MCFuncAddToFunc"))
			{
				referenceToMCFunc->Commands.push_back(MCCommand("say hi", MCVersion));
			}
			ImGui::Separator();
			for (size_t i = 0; i < referenceToMCFunc->Commands.size(); ++i)
			{
				 ImGui::Text(referenceToMCFunc->Commands[i].command.c_str());
				 ImGui::Separator();
			}
		}
		WindowElements::Dockable::End();
		/*
		TODO: TOOLBAR
		WindowElements::Dockable::Begin("Real Freakin Jank Right Here", &showThis, {0,0}, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		{
			if (ImGui::Button("Save##Internals_Save"))
			{
				std::ofstream thisFile;
				thisFile.open(fileDir);
				thisFile << referenceToMCFunc->MakeRawString();
				thisFile.close();
			}
		}
		WindowElements::Dockable::End();
		*/
	}
};