#pragma once

#include <vector>
#include <string>
#include "../Exceptions/NoNamespacesException.h"
#include "../Exceptions/NotImplementedException.h"
#include "MCFunction.h"

class MCDatapackProject
{
public:
	std::string ProjectName;
	std::string ProjectRootDirectory;
	std::string ProjectDescription;

	std::vector<std::string> Namespaces;
	std::vector<std::string> Files;

	std::vector<MCFunction> Functions;

	std::string ProjectMCVersion = "Undefined";
	std::string ProjectMCDPCVersion = "Undefined";

	MCDatapackProject()
	{
	}

	inline std::string MainNamespace() 
	{
		if (!Namespaces.empty()) 
			return Namespaces[1]; 
		else 
			throw new NoNamespacesException(); 
	}


private:


};
