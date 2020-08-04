#pragma once

#include "Builders/Window_Dockable.h"
#include "EditorWindow.h"
#include "../../MCDPProject/MCDatapackProject.hpp"

struct EditorProjectExplorer : EditorWindow
{
	MCDatapackProject* currentDatapackProject = nullptr;
	
	REGISTER_EDITOR_WINDOW(EditorProjectExplorer, "Project Explorer");

	std::string out_selectedFile;
	bool OpenFile = false;

	std::string Focussed;

	void Show() override
	{
		if (!currentDatapackProject)
		{
			ShowThis = false;
			return;
		}
		if (!out_selectedFile.empty())
		{
			OpenFile = false;
			out_selectedFile = "";
		}
		WindowElements::Dockable::Begin(DisplayWindowName, UniqueWindowID, &ShowThis);
		{
			for (size_t i = 0; i < currentDatapackProject->Files.size(); ++i)
			{
				if (ImGui::Selectable(currentDatapackProject->Files[i].c_str()))
				{
					if (Focussed.empty())
						Focussed = currentDatapackProject->Files[i].c_str();
					else if (Focussed == currentDatapackProject->Files[i].c_str())
					{
						if (std::filesystem::is_directory(currentDatapackProject->Files[i])) 
						{
							//TODO: OPEN A DIRECTORY
						}

						Messenger::push("OpenProjectExplorerFile", UniqueWindowID);
						OpenFile = true;
						out_selectedFile = currentDatapackProject->ProjectRootDirectory + "/" + currentDatapackProject->Files[i];
						Focussed = "";
					}
					else
					{
						Focussed = currentDatapackProject->Files[i].c_str();
					}
				}
			}
		}
		WindowElements::Dockable::End();
	}

};
