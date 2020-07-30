#pragma once

struct EditorWindow
{
	virtual void Show() = 0;
	const char* UniqueWindowID = "Undefined";
	const char* DisplayWindowName = "Undefined";
	bool ShowThis;
};
