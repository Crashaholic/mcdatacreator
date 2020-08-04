
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "EditorCode/Editor.hpp"

GLFWwindow* window;
double deltaTime = 0.0;
double lastFrame = 0.0;

/*
TODO:

EDITOR SETTINGS FILE (.editorsettings)

CONTAINS:

SIZE OF WINDOW
STYLE COLORS


...AND MORE!
*/

#define STYLE_VEC4_COLOUR(r, g, b, a) ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f)

#define STYLE_PRIM_COLOUR STYLE_VEC4_COLOUR(46.f, 125.f, 50.f, 255.f)
#define STYLE_BACKGROUND_COLOUR STYLE_VEC4_COLOUR(73.f, 73.f, 73.f, 255.f)

#define LIGHTEN(colour, amt) ImLerp(colour, ImVec4(1, 1, 1, 1), amt)
#define DARKEN(colour, amt) ImLerp(colour, ImVec4(0, 0, 0, 1), amt)

#define STYLE_TEXT ImVec4(1, 1, 1, 1)


int main()
{
	{
		/* Initialize the library */
		if (!glfwInit())
		{
			return -1;
		}

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow
		(
			800, 600, "MCDataCreator",
			NULL, NULL
		);

		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		Editor Persistent(window);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowRounding = 0.01f;
		style->WindowMenuButtonPosition = ImGuiDir_Right;
		style->WindowPadding = { 5.f, 5.f };
		style->FramePadding = { 5.f, 5.f };
		style->WindowTitleAlign = { 0.00f, 0.5f };
		style->WindowBorderSize = 1.0f;
		style->TabRounding = 0.0f;

		ImVec4* colors = style->Colors;
		colors[ImGuiCol_Text] =						STYLE_TEXT;
		colors[ImGuiCol_TextDisabled] =				DARKEN(STYLE_TEXT, 0.50f);
		colors[ImGuiCol_WindowBg] =					STYLE_BACKGROUND_COLOUR;
		colors[ImGuiCol_ChildBg] =					DARKEN(STYLE_BACKGROUND_COLOUR, 0.15f);
		colors[ImGuiCol_PopupBg] =					DARKEN(STYLE_BACKGROUND_COLOUR, 0.25f);
		colors[ImGuiCol_Border] =					LIGHTEN(STYLE_BACKGROUND_COLOUR, 0.30f);
		colors[ImGuiCol_BorderShadow] =				ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
		colors[ImGuiCol_FrameBg] =					DARKEN(STYLE_BACKGROUND_COLOUR, 0.4f);
		colors[ImGuiCol_FrameBgHovered] =			ImLerp(DARKEN(STYLE_BACKGROUND_COLOUR, 0.4f), STYLE_PRIM_COLOUR, 0.5f);
		colors[ImGuiCol_FrameBgActive] =			ImLerp(DARKEN(STYLE_BACKGROUND_COLOUR, 0.4f), DARKEN(STYLE_PRIM_COLOUR, 0.2f), 0.5f);
		colors[ImGuiCol_TitleBg] =					STYLE_BACKGROUND_COLOUR;
		colors[ImGuiCol_TitleBgActive] =			LIGHTEN(STYLE_BACKGROUND_COLOUR, 0.15f);
		colors[ImGuiCol_TitleBgCollapsed] =			ImLerp(STYLE_BACKGROUND_COLOUR, ImVec4(0, 0, 0, 0), 0.35f);
		colors[ImGuiCol_MenuBarBg] =				STYLE_BACKGROUND_COLOUR;
		colors[ImGuiCol_ScrollbarBg] =				STYLE_VEC4_COLOUR(0, 0, 0, 0);
		colors[ImGuiCol_ScrollbarGrab] =			DARKEN(STYLE_BACKGROUND_COLOUR, 0.5f);
		colors[ImGuiCol_ScrollbarGrabHovered] =		LIGHTEN(DARKEN(STYLE_BACKGROUND_COLOUR, 0.5f), 0.2f);
		colors[ImGuiCol_ScrollbarGrabActive] =		DARKEN(DARKEN(STYLE_BACKGROUND_COLOUR, 0.5f), 0.2f);
		colors[ImGuiCol_CheckMark] =				STYLE_TEXT;
		colors[ImGuiCol_SliderGrab] =				LIGHTEN(STYLE_PRIM_COLOUR, 0.3f);
		colors[ImGuiCol_SliderGrabActive] =			DARKEN(LIGHTEN(STYLE_PRIM_COLOUR, 0.3f), 0.2f);
		colors[ImGuiCol_Button] =					ImVec4(0, 0, 0, 0.1f);
		colors[ImGuiCol_ButtonHovered] =			ImVec4(1, 1, 1, 0.2f);
		colors[ImGuiCol_ButtonActive] =				ImVec4(0, 0, 0, 0.4f);
		colors[ImGuiCol_Header] =					STYLE_BACKGROUND_COLOUR;
		colors[ImGuiCol_HeaderHovered] =			LIGHTEN(STYLE_BACKGROUND_COLOUR, 0.25f);
		colors[ImGuiCol_HeaderActive] =				DARKEN(STYLE_BACKGROUND_COLOUR, 0.25f);
		colors[ImGuiCol_Separator] =				DARKEN(STYLE_BACKGROUND_COLOUR, 0.3f);
		colors[ImGuiCol_SeparatorHovered] =			DARKEN(LIGHTEN(STYLE_BACKGROUND_COLOUR, 0.25f), 0.3f);
		colors[ImGuiCol_SeparatorActive] =			DARKEN(DARKEN(STYLE_BACKGROUND_COLOUR, 0.25f), 0.3f);
		colors[ImGuiCol_ResizeGrip] =				STYLE_PRIM_COLOUR;
		colors[ImGuiCol_ResizeGripHovered] =		LIGHTEN(STYLE_PRIM_COLOUR, 0.2f);
		colors[ImGuiCol_ResizeGripActive] =			DARKEN(STYLE_PRIM_COLOUR, 0.2f);
		colors[ImGuiCol_Tab] =						STYLE_BACKGROUND_COLOUR;
		colors[ImGuiCol_TabHovered] =				LIGHTEN(STYLE_PRIM_COLOUR, 0.30f);
		colors[ImGuiCol_TabActive] =				LIGHTEN(STYLE_PRIM_COLOUR, 0.17f);
		colors[ImGuiCol_TabUnfocused] =				STYLE_BACKGROUND_COLOUR;
		colors[ImGuiCol_TabUnfocusedActive] =		LIGHTEN(STYLE_BACKGROUND_COLOUR, 0.2f);
		colors[ImGuiCol_DockingEmptyBg] =			DARKEN(STYLE_BACKGROUND_COLOUR, 0.5f);
		colors[ImGuiCol_PlotLines] =				ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] =			ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] =			ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] =		ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] =			ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] =			ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] =				ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] =	ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] =		ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] =			ImVec4(0.00f, 0.00f, 0.00f, 0.305f);

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
		glfwSwapInterval(0);

		while (!glfwWindowShouldClose(window))
		{
			glEnable(GL_DEPTH_TEST);
			float currentFrame = (float)glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Persistent.Update();

			//ImGui::ShowDemoWindow();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	_CrtDumpMemoryLeaks();
}