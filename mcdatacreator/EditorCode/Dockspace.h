#pragma once

#include "DockspaceMenu.h"

struct
{
	void Update()
	{

		// This function just does the dockspace for the window
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Docking Space", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");

		if (!ImGui::DockBuilderGetNode(dockspace_id))
		{
			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);

			ImGuiID dock_main_id = dockspace_id;
			//ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.05f, nullptr, &dock_main_id);
			//ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);

			// Disable tab bar for custom toolbar
			//ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_up_id);
			//node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

			ImGui::DockBuilderFinish(dock_main_id);
		}

		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			DockspaceMenu.Update();
		}
		ImGui::EndMenuBar();
		ImGui::End();
		// EOF
	}
} Dockspace ;