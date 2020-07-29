#pragma once

#include <exception>

class NoNamespacesException : public std::exception
{
public:
	const char* what() { return "Current project has no namespaces!"; }
};