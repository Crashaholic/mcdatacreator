#pragma once

#include "MCSelectorType.h"
#include "MCPosition.h"
#include "MCJsonText.h"

class MCCommand
{
public:

	MCCommand(std::string s, std::string MCVersion = "")
	{
		std::string copyOfString = s;
		std::string delimiter = " ";

		size_t pos = 0;
		std::string token;
		while ((pos = copyOfString.find(delimiter)) != std::string::npos) {
			token = copyOfString.substr(0, pos);
			arguments.push_back(token);
			copyOfString.erase(0, pos + delimiter.length());
		}
		arguments.push_back(copyOfString);
		command = arguments[0];
	}

	std::string command;
	std::vector<MCSelector> selector;
	std::vector<MCPosition> position;
	std::vector<MCJsonText> jsonText;
	std::vector<std::string> arguments;

	std::string MakeRawString()
	{
		std::string ret = "";
		for (size_t i = 0; i < arguments.size(); ++i)
		{
			ret += arguments[i];
			if (i < arguments.size() - 1)
				ret += ' ';
			else
				ret += '\n';
		}
		return ret;
	}

};
