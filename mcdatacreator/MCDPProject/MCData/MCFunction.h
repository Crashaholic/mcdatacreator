#pragma once

#include <vector>
#include "../MCCommand.h"

struct MCFunction
{
	std::string Name;
	std::string RawString;
	std::string UUID; // TODO: LOOK INTO GUIDs, MIGHT BE HANDY DOWN THE LINE https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library
	std::vector<MCCommand> Commands;
	std::vector<MCFunction> ReferencedFunctions; // might be useful in the future for deletion and such.

	MCFunction(std::string name, std::string directory = "")
	{
		if (directory.empty())
			Name = name;
		else
		{
			// AUTOMATICALLY GENERATE THE FUNCTION NAME
			Name = name;
		}
	}

	void ParseFromRawString()
	{
		if (RawString.empty())
			return;

		std::string s = RawString;
		std::string delimiter = "\n";

		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			Commands.push_back(MCCommand(token));
			s.erase(0, pos + delimiter.length());
		}
		Commands.push_back(MCCommand(s));
	}

	std::string MakeRawString()
	{
		std::string ret;
		for (size_t i = 0; i < Commands.size(); ++i)
		{
			ret += Commands[i].MakeRawString();
		}
		return ret;
	}
};
