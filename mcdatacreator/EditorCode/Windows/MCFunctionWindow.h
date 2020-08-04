#pragma once

#include "Builders/Window_Dockable.h"
#include "Builders/WindowElements.h"
#include "../../MCDPProject/MCData/MCFunction.h"
#include "EditorWindow.h"

struct MCFunctionWindow : EditorWindow
{
	MCFunction* referenceToMCFunc = nullptr;
	bool EditRaw = false;
	bool isFocussed = false;

	std::string fileDir = "";
	std::string fileName = "";
	std::string MCVersion = "";

	std::string fileContents = "";
	
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
		}
		else if (!referenceToMCFunc || fileName.empty())
		{
			ShowThis = false;
			return;
		}
		UniqueWindowID = DisplayWindowName = fileName.c_str();
		WindowElements::Dockable::Begin((fileName).c_str(), UniqueWindowID, &ShowThis);
		{
			if (Messenger::eat("FileAttemptSave"))
			{
				std::cout << "saving file: " << UniqueWindowID << '\n';
				std::ofstream thisFile;
				thisFile.open(fileDir);
				thisFile << referenceToMCFunc->MakeRawString();
				thisFile.close();
			}

			if (Messenger::back().first == "FileShowRawText")
			{
				if (Messenger::back().second == this->UniqueWindowID)
				{
					referenceToMCFunc->RawString = referenceToMCFunc->MakeRawString();
					EditRaw = true;
					Messenger::pop();
				}
			}
			else if (Messenger::back().first == "FileShowPrettyText")
			{
				if (Messenger::back().second == this->UniqueWindowID)
				{
					referenceToMCFunc->Commands.clear();
					referenceToMCFunc->ParseFromRawString();
					EditRaw = false;
					Messenger::pop();
				}
			}

			if (ImGui::IsWindowFocused() && !isFocussed)
			{
				if (EditRaw) 
					Messenger::push("IsRawView", UniqueWindowID);
				else 
					Messenger::push("IsNotRawView", UniqueWindowID);
				//Messenger::push("WindowIs_MCFunc", UniqueWindowID);
				Messenger::push("FocusWindow", UniqueWindowID);
				isFocussed = true;
			}
			else if (ImGui::IsWindowFocused())
			{
				isFocussed = true;
			}
			else
			{
				isFocussed = false;
			}

			if (EditRaw)
			{
				ImVec2 vMin = ImGui::GetWindowContentRegionMin();
				ImVec2 vMax = ImGui::GetWindowContentRegionMax();
				ImGui::PushItemWidth(-1);
				ImGui::InputTextMultiline("##Text", &referenceToMCFunc->RawString, ImVec2(vMax.x - vMin.x, vMax.y - vMin.y));
				ImGui::PopItemWidth();
			}
			else
			{
				// TODO: Add to mcfunc button is phenomenal

				if (ImGui::Button("Add##MCFuncAddToFunc"))
				{
					referenceToMCFunc->Commands.push_back(MCCommand("say hi", MCVersion));
				}
				ImGui::Separator();
				for (size_t i = 0; i < referenceToMCFunc->Commands.size(); ++i)
				{
					if (ImGui::CollapsingHeader(referenceToMCFunc->Commands[i].command.c_str()))
					{

					}
					ImGui::Separator();
				}
			}

		}
		WindowElements::Dockable::End();
	}
};