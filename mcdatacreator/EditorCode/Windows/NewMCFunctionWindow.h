#pragma once

#include "Window_NoDock.h"
#include "WindowElements.h"
#include "../DockspaceMenu.h"

struct
{
	bool DoNewMCFunction = false;
	void Show()
	{
		WindowElements::NoDock::Begin("New MC Function##NewMCFuncWindow", &DockspaceMenu.showNewMCFunctionWindow);
		{
			if (ImGui::Button("Create##NewMCFuncBtn"))
			{
				DoNewMCFunction = true;
			}
		}
		WindowElements::NoDock::End();
	}
} NewMCFunctionWindow ;