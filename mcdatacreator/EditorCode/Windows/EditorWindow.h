#pragma once

struct EditorWindow
{
	virtual void Show() = 0;
	const char* UniqueWindowID = "Undefined";
	const char* DisplayWindowName = "Undefined";
	bool ShowThis = false;
};

#define REGISTER_EDITOR_WINDOW(WINDOW_ID, WINDOW_DISPLAY_NAME) WINDOW_ID() { UniqueWindowID = #WINDOW_ID; DisplayWindowName = WINDOW_DISPLAY_NAME; ShowThis = true; }
