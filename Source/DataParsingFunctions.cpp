/*
	DiscordCoreLoader, A bot library for Discord, written in C++, and featuring explicit multithreading through the usage of custom, asynchronous C++ CoRoutines.

	Copyright 2021, 2022 Chris M. (RealTimeChris)

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
	USA
*/
/// DataParsingFunctions.cpp - Source file for the data-parsing functions.
/// Oct 1, 2021
/// https://discordcoreapi.com
/// \file DataParsingFunctions.cpp

#include <discordcoreloader/DataParsingFunctions.hpp>

namespace DiscordCoreLoader {

	JsonParseError::JsonParseError(int32_t theCode) : std::runtime_error(theErrors[theCode]){};

	uint64_t getId(simdjson::ondemand::value jsonData, const char* theKey) {
		return uint64_t{ stoull(getString(jsonData, theKey)) };
	}

	float getFloat(simdjson::ondemand::value jsonData, const char* theKey) {
		double theValue{};
		if (jsonData[theKey].get(theValue) == simdjson::error_code::SUCCESS) {
			return double{ theValue };
		} else {
			return 0.0f;
		}
	}

	bool getbool(simdjson::ondemand::value jsonData, const char* theKey) {
		bool theValue{};
		if (jsonData[theKey].get(theValue) == simdjson::error_code::SUCCESS) {
			return bool{ theValue };
		} else {
			return false;
		}
	}

	uint8_t getUint8(simdjson::ondemand::value jsonData, const char* theKey) {
		uint64_t theValue{};
		if (jsonData[theKey].get(theValue) == simdjson::error_code::SUCCESS) {
			return static_cast<uint8_t>(theValue);
		} else {
			return 0;
		}
	}

	uint16_t getUint16(simdjson::ondemand::value jsonData, const char* theKey) {
		uint64_t theValue{};
		if (jsonData[theKey].get(theValue) == simdjson::error_code::SUCCESS) {
			return static_cast<uint16_t>(theValue);
		} else {
			return 0;
		}
	}

	uint32_t getUint32(simdjson::ondemand::value jsonData, const char* theKey) {
		uint64_t theValue{};
		if (jsonData[theKey].get(theValue) == simdjson::error_code::SUCCESS) {
			return static_cast<uint32_t>(theValue);
		} else {
			return 0;
		}
	}

	uint64_t getUint64(simdjson::ondemand::value jsonData, const char* theKey) {
		uint64_t theValue{};
		if (jsonData[theKey].get(theValue) == simdjson::error_code::SUCCESS) {
			return uint64_t{ theValue };
		} else {
			return 0;
		}
	}

	std::string getString(simdjson::ondemand::value jsonData, const char* theKey) {
		std::string_view theValue{};
		if (jsonData[theKey].get(theValue) == simdjson::error_code::SUCCESS) {
			return std::string{ theValue.data(), theValue.size() };
		} else {
			return "";
		}
	}

	ObjectReturnData getObject(ArrayReturnData jsonData, uint64_t objectIndex) {
		ObjectReturnData theValue{};
		if (jsonData.didItSucceed && jsonData.theArray.at(objectIndex).get(theValue.theObject) == simdjson::error_code::SUCCESS) {
			theValue.didItSucceed = true;
		}
		return theValue;
	}

	ObjectReturnData getObject(ObjectReturnData jsonData, const char* objectName) {
		ObjectReturnData theValue{};
		if (jsonData.didItSucceed && jsonData.theObject[objectName].get(theValue.theObject) == simdjson::error_code::SUCCESS) {
			theValue.didItSucceed = true;
		}
		return theValue;
	}

	ArrayReturnData getArray(ObjectReturnData jsonData, const char* arrayName) {
		ArrayReturnData theValue{};
		if (jsonData.didItSucceed && jsonData.theObject[arrayName].get(theValue.theArray) == simdjson::error_code::SUCCESS) {
			theValue.didItSucceed = true;
		}
		return theValue;
	}

	ObjectReturnData getObject(simdjson::ondemand::array jsonData, uint64_t objectIndex) {
		ObjectReturnData theValue{};
		if (jsonData.at(objectIndex).get(theValue.theObject) == simdjson::error_code::SUCCESS) {
			theValue.didItSucceed = true;
		}
		return theValue;
	}

	ObjectReturnData getObject(simdjson::ondemand::value jsonData, const char* objectName) {
		ObjectReturnData theValue{};
		if (jsonData[objectName].get(theValue.theObject) == simdjson::error_code::SUCCESS) {
			theValue.didItSucceed = true;
		}
		return theValue;
	}

	ArrayReturnData getArray(simdjson::ondemand::value jsonData, const char* arrayName) {
		ArrayReturnData theValue{};
		if (jsonData[arrayName].get(theValue.theArray) == simdjson::error_code::SUCCESS) {
			theValue.didItSucceed = true;
		}
		return theValue;
	}

	std::string getString(ObjectReturnData jsonData, const char* theKey) {
		std::string_view theValue{};
		if (jsonData.didItSucceed && jsonData.theObject[theKey].get(theValue) == simdjson::error_code::SUCCESS) {
			return static_cast<std::string>(theValue);
		}
		return static_cast<std::string>(theValue);
	}

};
