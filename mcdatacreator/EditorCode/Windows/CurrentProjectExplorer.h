#pragma once

#include "Builders/Window_Dockable.h"
#include "../../MCDPProject/MCDatapackProject.hpp"

struct
{
	MCDatapackProject* currentDatapackProject = nullptr;
	bool showThis;

	void Show()
	{
		if (!currentDatapackProject)
		{
			showThis = false;
			return;
		}

		WindowElements::Dockable::Begin("Project Explorer##EditorProjectExplorer");
		{

		}
		WindowElements::Dockable::End();
	}

} CurrentProjectExplorer;
