#pragma once

#include <string>
#include "json\json.hpp"

struct MCBlock
{
	// MANDATORY
	std::string Name;
	// USE FLATTENED VALUES (FLATTENING HAPPENED IN 1.13, SO SHOULD BE USING THEM ANYWAYS)
	std::string MCInternalID;
	// CAN BE NULL, OTHERWISE MUST BE SET MANUALLY (NOT SYNCED WITH BlockEntityData_RawJsonText)
	nlohmann::json BlockEntityData;
	// RAW ENTITY DATA TEXT, IF USER CHOOSES TO USE THIS (NOT SYNCED WITH BlockEntityData)
	std::string BlockEntityData_RawJsonText;

	MCBlock()
	{
		Name = "EMPTY_NAME";
		MCInternalID = "EMPTY_ID";
	}

	MCBlock(std::string BlockName, std::string BlockInternalID)
	{
		Name = BlockName;
		MCInternalID = BlockInternalID;
	}
};

