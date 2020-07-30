#pragma once

#include "Builders/Window_Dockable.h"
#include "Builders/Window_NoDock.h"
#include "Builders/WindowElements.h"
#include "../../MCDPProject/MCFunction.h"

#include "ShowWindowBools.h"

struct MCFunctionWindow
{
	bool showThis = false;
	MCFunction* referenceToMCFunc = nullptr;
	std::string fileDir = "";
	std::string fileName = "";
	std::string MCVersion = "";
	
	void Show()
	{
		if (!referenceToMCFunc || fileName.empty())
		{
			showThis = false;
			return;
		}
		WindowElements::Dockable::Begin((fileName).c_str(), &showThis);
		{
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
	}
} CurrentMCFunctionWindow;