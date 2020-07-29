#pragma once

#include <exception>

class NotImplementedException : public std::exception
{
public:
	const char* what()
	{
		return "Thing is defined but there's no implementation!";
	}
};
