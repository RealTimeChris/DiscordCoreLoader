/*
*
	DiscordCoreLoader, A stress-tester for Discord bot libraries, and Discord bots.

	Copyright 2022 Chris M. (RealTimeChris)

	This file is part of DiscordCoreLoader.
	DiscordCoreLoader is free software: you can redistribute it and/or modify it under the terms of the GNU
	General Public License as published by the Free Software Foundation, either version 3 of the License,
	or (at your option) any later version.
	DiscordCoreLoader is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
	even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
	You should have received a copy of the GNU General Public License along with DiscordCoreLoader.
	If not, see <https://www.gnu.org/licenses/>.

*/
/// DataParsingFunctions.hpp - Header file for the data-parsing functions.
/// Oct 1, 2021
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file DataParsingFunctions.hpp

#pragma once

#ifndef DATA_PARSING_FUNCTIONS
	#define DATA_PARSING_FUNCTIONS
	#include <discordcoreloader/FoundationEntities.hpp>
	#include <simdjson.h>
	#include <map>

namespace DiscordCoreLoader {

	struct ObjectReturnData {
		simdjson::ondemand::value object{};
		bool didItSucceed{ false };
	};

	struct ArrayReturnData {
		simdjson::ondemand::array arrayValue{};
		bool didItSucceed{ false };
	};

	Snowflake getId(simdjson::ondemand::value jsonObjectData, const char* key);

	Snowflake getId(simdjson::ondemand::value jsonObjectData);

	int64_t getInt64(simdjson::ondemand::value jsonData, const char* key);

	int32_t getInt32(simdjson::ondemand::value jsonData, const char* key);

	int16_t getInt16(simdjson::ondemand::value jsonData, const char* key);

	int8_t getInt8(simdjson::ondemand::value jsonData, const char* key);

	uint64_t getUint64(simdjson::ondemand::value jsonData, const char* key);

	uint32_t getUint32(simdjson::ondemand::value jsonData, const char* key);

	uint16_t getUint16(simdjson::ondemand::value jsonData, const char* key);

	uint8_t getUint8(simdjson::ondemand::value jsonData, const char* key);

	float getFloat(simdjson::ondemand::value jsonData, const char* key);

	bool getBool(simdjson::ondemand::value jsonData, const char* key);

	std::string getString(simdjson::ondemand::value jsonData, const char* key);

	std::string getString(ObjectReturnData jsonData, const char* key);

	std::string getString(simdjson::ondemand::value jsonData);

	std::string getString(ObjectReturnData jsonData, const char* key);

	ObjectReturnData getObject(simdjson::ondemand::value jsonObjectData, const char* objectName);

	ObjectReturnData getObject(ObjectReturnData jsonObjectData, const char* objectName);

	ObjectReturnData getObject(ArrayReturnData jsonObjectData, uint64_t objectIndex);

	ArrayReturnData getArray(simdjson::ondemand::value jsonObjectData, const char* arrayName);

	ArrayReturnData getArray(ObjectReturnData jsonObjectData, const char* arrayName);

};
#endif