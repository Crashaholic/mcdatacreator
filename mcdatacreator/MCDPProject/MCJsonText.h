#pragma once

#include <json/json.hpp>
#include <string>
#include <sstream>

using std::string;
using Json = nlohmann::json;

class MCJsonText
{
public:

	string text;
	string color;

	inline std::string ConvertToString() 
	{
		Json j;

		j[""]["text"] = text;
		if (!color.empty())
		{
			j[""]["color"] = color;
		}
		std::stringstream ss;
		ss << j;
		return ss.str();
	}

private:
};


