#pragma once

#include "../EditorMessenger.h"

struct EditorWindow
{
	virtual void Show() = 0;
	const char* UniqueWindowID = "Undefined";
	const char* DisplayWindowName = "Undefined";
	bool ShowThis = false;
};

#define REGISTER_EDITOR_WINDOW(WINDOW_ID, WINDOW_DISPLAY_NAME) WINDOW_ID() { UniqueWindowID = #WINDOW_ID; DisplayWindowName = WINDOW_DISPLAY_NAME; ShowThis = true; }
/*
Returns true if valid.
Returns false if invalid.
*/
bool ValidateNamespace(std::string* string_to_validate)
{
	for (size_t i = 0; i < string_to_validate->size(); ++i)
	{
		char currChar = (*string_to_validate)[i];
		if (std::isalpha(currChar))
		{
			if (std::isupper(currChar))
			{
				return false;
			}
		}
		else if (currChar != '_' && currChar != '-' && !std::isdigit(currChar))
		{
			return false;
		}
	}
	return true;
}
