#pragma once

#include "Builders/Window_Dockable.h"
#include "Builders/WindowElements.h"
#include "EditorWindow.h"

struct AdvancementsWindow : EditorWindow
{
	MCFunction* referenceToMCFunc = nullptr;
	bool EditRaw = false;
	bool isFocussed = false;

	std::string fileDir = "";
	std::string fileName = "";
	std::string MCVersion = "";

	std::string fileContents = "";

	AdvancementsWindow()
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
			}
		}
	}
};
