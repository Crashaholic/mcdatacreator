#pragma once

#include <unordered_map>
#include <typeindex>
#include <typeinfo>

#include "Windows/EditorWindow.h"

struct WindowStack
{
	std::unordered_map<std::string, EditorWindow*> windows;

	~WindowStack()
	{
		if (windows.size() > 0)
		{
			for (auto const& x : windows)
			{
				delete x.second;
			}
		}
	}

	template <class Window>
	Window* get()
	{
		if (windows.find(typeid(Window).name()) != windows.end())
		{
			EditorWindow* ew = windows[typeid(Window).name()];
			if (ew == NULL)
				return NULL;
			return static_cast<Window*>(ew);

		}
		return nullptr;
	}

	template <class Window>
	Window* add()
	{
		auto temp = get<Window>();
		if (!temp)
		{
			Window* wType = new Window;
			windows[typeid(Window).name()] = wType;
			return wType;
		}
		return temp;
	}

	/// For explicit removal
	template <class Window>
	bool rem()
	{
		if (get<Window>())
		{
			delete windows[typeid(Window).name()];
			windows[typeid(Window).name()] = nullptr;
			windows.erase(windows[typeid(Window).name()]);
			return true;
		}
		return false;
	}

	bool rem(std::string s)
	{
		if (windows.find(s) != windows.end())
		{
			delete windows[s];
			windows[s] = nullptr;
			windows.erase(s);
			return true;
		}
		return false;
	}

	/// Special cases (i.e. Windows I will have a lot of. E.g. Function, crafting, etc..)
	template <class Window>
	Window* add(std::string key)
	{
		auto temp = get<Window>();
		if (!temp)
		{
			Window* wType = new Window;
			windows[key] = wType;
			return wType;
		}
		return temp;
	}

	/// Special cases (i.e. Windows I will have a lot of. E.g. Function, crafting, etc..)
	template <class Window>
	Window* get(std::string key)
	{
		if (windows.find(key) != windows.end())
		{
			EditorWindow* ew = windows[key];
			if (ew == NULL)
				return NULL;
			return static_cast<Window*>(ew);
		}
		return nullptr;
	}

	void show()
	{
		for (auto const& [key, val] : windows)
		{
			val->Show();
		}
	}

	void check()
	{
		std::vector<std::string> marked_delete;
		std::unordered_map<std::string, EditorWindow*>::iterator it;
		for (it = windows.begin(); it != windows.end(); it++)
		{
			if (!it->second->ShowThis)
			{
				marked_delete.push_back(it->first);
			}
		}

		for (size_t i = 0; i < marked_delete.size(); ++i)
		{
			this->rem(marked_delete[i]);
		}
	}
}winStack;

