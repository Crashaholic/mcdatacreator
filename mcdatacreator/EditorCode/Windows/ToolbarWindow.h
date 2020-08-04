#pragma once

#include "Builders\Window_Dockable.h"
#include "EditorWindow.h"
#include "../../MCDPProject/MCDatapackProject.hpp"

struct ToolbarWindow : EditorWindow
{
	REGISTER_EDITOR_WINDOW(ToolbarWindow, "Toolbar");

	std::string FocussedFile = "";
	std::string contents = "";

	bool AttemptSave = false;
	bool RawView = false;

	void Show() override
	{
		WindowElements::Dockable::Begin(DisplayWindowName, UniqueWindowID, &ShowThis, {60, 50}, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
		{
			if (FocussedFile.empty())
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			if (ImGui::Button("Save##ToolbarSave"))
			{
				Messenger::push("FileAttemptSave", UniqueWindowID);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Raw View##ToolbarRawView", &RawView))
			{
				Messenger::push(RawView ? "FileShowRawText" : "FileShowPrettyText", FocussedFile);
			}
			ImGui::SameLine();
			if (FocussedFile.empty())
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
		}
		WindowElements::Dockable::End();
	}
};
