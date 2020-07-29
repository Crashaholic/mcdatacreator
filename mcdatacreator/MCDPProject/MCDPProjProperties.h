#pragma once

#include <string>
#include <vector>
#include "json/json.hpp"

struct MCDPPRojProperties
{
	std::string projectName; // Name of the project
	std::string projectDirectory; // Directory to the project (might be redundant)
	std::string projectMCVersion; // Version of MC this datapack is intended for
	std::string projectMCDPCVersion; // Version of MC Data Creator used
	std::vector<std::string> Namespaces; // Namespaces in the current project
	std::vector<std::string> Files; // Files in the current project (might be redundant)

	unsigned long long activeNamespaceIndex; // The namespace last used by the user
	unsigned long long activeFileIndex; // The file last opened by the user

	bool CheckIntegrity()
	{
		return !(projectName.empty() || projectDirectory.empty() || projectMCVersion.empty() || projectMCDPCVersion.empty() || Namespaces.empty());
	}
};
