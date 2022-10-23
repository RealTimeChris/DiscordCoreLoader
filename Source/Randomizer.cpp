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
/// Randomizer.cpp - Source file for the Randomizer stuff.
/// May 22, 2022
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file Randomizer.cpp

#include <discordcoreloader/Randomizer.hpp>

namespace DiscordCoreLoader {

	template<typename ReturnType> std::string toHex(ReturnType inputValue) {
		std::stringstream theStream{};
		theStream << std::setfill('0') << std::setw(sizeof(ReturnType) * 2) << std::hex << inputValue;
		return theStream.str();
	}

	std::string Randomizer::randomizeIconHash() {
		uint64_t theValue01 = this->randomize64BitUInt();
		uint64_t theValue02 = this->randomize64BitUInt();

		std::string returnString = toHex(theValue01);
		std::string returnStringNew{};

		for (uint32_t x = 0; x < returnString.size(); x++) {
			auto newValue = returnString[x];
			returnStringNew.push_back(newValue);
		}
		returnString = toHex(theValue02);
		for (uint32_t x = 0; x < returnString.size(); x++) {
			auto newValue = returnString[x];
			returnStringNew.push_back(newValue);
		}
		returnStringNew.resize(32);
		return returnStringNew;
	}

	std::string Randomizer::randomizeString(int64_t length) {
		std::string returnString{};
		for (uint32_t x = 0; x < length; x++) {
			auto theValue =
				static_cast<char>((static_cast<float>(this->randomEngine()) / static_cast<float>(this->randomEngine.max()) * 93.0f) + 35.0f);
			if (static_cast<char>(theValue) == static_cast<char>(',') || static_cast<char>(theValue) == '\'' || static_cast<char>(theValue) == '/' ||
				static_cast<char>(theValue) == '\"' || static_cast<char>(theValue) == '\\' || static_cast<char>(theValue) == '`' ||
				static_cast<char>(theValue) == '{' || static_cast<char>(theValue) == '}' || static_cast<char>(theValue) == '[' ||
				static_cast<char>(theValue) == ']' || static_cast<char>(theValue) == '(' || static_cast<char>(theValue) == ')' ||
				static_cast<char>(theValue) == static_cast<char>(0) || static_cast<char>(theValue) == static_cast<char>(1)) {
				theValue = static_cast<char>('s');
			}
			returnString.push_back(theValue);
		}
		return returnString;
	}

	void Randomizer::randomizeId(std::string& theString, uint64_t minValue, uint64_t maxValue) {
		std::string returnString{};
		returnString.resize(20);
		auto theValue = this->randomize64BitUInt(minValue, maxValue);
		std::to_chars(returnString.data(), returnString.data() + returnString.size(), theValue);
		if (returnString.size() > 18) {
			uint64_t length = returnString.size() - 18;
			theString = std::move(returnString.substr(0, returnString.size() - length));
			for (int32_t x = 0; x < theString.size(); ++x) {
				if (static_cast<char>(theString[x]) == static_cast<char>(',') || static_cast<char>(theString[x]) == '\'' ||
					static_cast<char>(theString[x]) == '/' || static_cast<char>(theString[x]) == '\"' || static_cast<char>(theString[x]) == '\\' ||
					static_cast<char>(theString[x]) == '`' || static_cast<char>(theString[x]) == '{' || static_cast<char>(theString[x]) == '}' ||
					static_cast<char>(theString[x]) == '[' || static_cast<char>(theString[x]) == ']' || static_cast<char>(theString[x]) == '(' ||
					static_cast<char>(theString[x]) == ')' || static_cast<char>(theString[x]) == static_cast<char>(0) ||
					static_cast<char>(theString[x]) == static_cast<char>(1)) {
					theString[x] = static_cast<char>(' ');
				}
			}
		}
	}

	uint64_t Randomizer::randomize64BitUInt(double mean, double stdDeviation) {
		std::normal_distribution<> normalDistributionTwo{ mean, stdDeviation };
		auto theResult = normalDistributionTwo(this->randomEngine);
		if (theResult < 0) {
			theResult = -theResult;
		}
		return static_cast<uint64_t>(theResult);
	}

	uint64_t Randomizer::randomize64BitUInt(uint64_t minValue, uint64_t maxValue) {
		double difference = maxValue - minValue;
		uint64_t theValue =
			static_cast<uint64_t>(static_cast<double>(this->randomEngine()) / static_cast<double>(this->randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	uint32_t Randomizer::randomize32BitUInt(uint32_t minValue, uint32_t maxValue) {
		double difference = maxValue - minValue;
		uint32_t theValue =
			static_cast<uint32_t>(static_cast<double>(this->randomEngine()) / static_cast<double>(this->randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	uint16_t Randomizer::randomize16BitUInt(uint16_t minValue, uint16_t maxValue) {
		double difference = maxValue - minValue;
		uint16_t theValue =
			static_cast<uint16_t>(static_cast<double>(this->randomEngine()) / static_cast<double>(this->randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	uint8_t Randomizer::randomize8BitUInt(uint8_t minValue, uint8_t maxValue) {
		double difference = maxValue - minValue;
		uint8_t theValue =
			static_cast<uint8_t>(static_cast<double>(this->randomEngine()) / static_cast<double>(this->randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	int8_t Randomizer::randomize8BitInt(uint8_t minValue, uint8_t maxValue) {
		double difference = maxValue - minValue;
		int8_t theValue =
			static_cast<int8_t>(static_cast<double>(this->randomEngine()) / static_cast<double>(this->randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	uint64_t Randomizer::drawRandomValue(std::vector<uint64_t>& theValues) {
		if (theValues.size() == 0) {
			return 0;
		}
		uint32_t valueIndex = static_cast<uint32_t>(
			(static_cast<float>(this->randomEngine()) / static_cast<float>(this->randomEngine.max())) * static_cast<float>(theValues.size()));
		return theValues[valueIndex];
	}

	uint64_t Randomizer::randomize64BitUInt() {
		return static_cast<uint64_t>(this->randomEngine());
	}

	uint32_t Randomizer::randomize32BitUInt() {
		return static_cast<uint32_t>(this->randomEngine());
	}

	uint16_t Randomizer::randomize16BitUInt() {
		return static_cast<uint16_t>(this->randomEngine());
	}

	double Randomizer::randomize64BitFloat() {
		return static_cast<double>(this->randomEngine());
	}

	uint8_t Randomizer::randomize8BitUInt() {
		auto theValue = static_cast<uint8_t>(this->randomEngine());
		if (static_cast<char>(theValue) == static_cast<char>(',') || static_cast<char>(theValue) == '\'' || static_cast<char>(theValue) == '/' ||
			static_cast<char>(theValue) == '"' || static_cast<char>(theValue) == '\\' || static_cast<char>(theValue) == '`' ||
			static_cast<char>(theValue) == '{' || static_cast<char>(theValue) == '}' || static_cast<char>(theValue) == '[' ||
			static_cast<char>(theValue) == ']' || static_cast<char>(theValue) == '(' || static_cast<char>(theValue) == ')' ||
			static_cast<char>(theValue) == static_cast<char>(0) || static_cast<char>(theValue) == static_cast<char>(1)) {
			theValue = static_cast<char>('s');
		}
		return theValue;
	}

	int64_t Randomizer::randomize64BitInt() {
		return static_cast<int64_t>(this->randomEngine());
	}

	int32_t Randomizer::randomize32BitInt() {
		return static_cast<int32_t>(this->randomEngine());
	}

	int16_t Randomizer::randomize16BitInt() {
		return static_cast<int16_t>(this->randomEngine());
	}

	float Randomizer::randomize32BitFloat() {
		return static_cast<float>(this->randomEngine());
	}

	int8_t Randomizer::randomize8BitInt() {
		auto theValue = static_cast<int8_t>(this->randomEngine());
		if (static_cast<char>(theValue) == static_cast<char>(',') || static_cast<char>(theValue) == '\'' || static_cast<char>(theValue) == '/' ||
			static_cast<char>(theValue) == '"' || static_cast<char>(theValue) == '\\' || static_cast<char>(theValue) == '`' ||
			static_cast<char>(theValue) == '{' || static_cast<char>(theValue) == '}' || static_cast<char>(theValue) == '[' ||
			static_cast<char>(theValue) == ']' || static_cast<char>(theValue) == '(' || static_cast<char>(theValue) == ')' ||
			static_cast<char>(theValue) == static_cast<char>(0) || static_cast<char>(theValue) == static_cast<char>(1)) {
			theValue = static_cast<char>('s');
		}
		return theValue;
	}

}