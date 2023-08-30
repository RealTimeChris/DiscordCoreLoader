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

	template<typename ReturnType> ContIterator::String toHex(ReturnType inputValue) {
		std::stringstream theStream{};
		theStream << std::setfill('0') << std::setw(sizeof(ReturnType) * 2) << std::hex << inputValue;
		return theStream.str();
	}

	ContIterator::String Randomizer::randomizeIconHash() {
		uint64_t theValue01 = randomize64BitUInt();
		uint64_t theValue02 = randomize64BitUInt();

		ContIterator::String returnString = toHex(theValue01);
		ContIterator::String returnStringNew{};

		for (uint32_t x = 0; x < returnString.size(); x++) {
			auto newValue = returnString[x];
			returnStringNew.pushBack(newValue);
		}
		returnString = toHex(theValue02);
		for (uint32_t x = 0; x < returnString.size(); x++) {
			auto newValue = returnString[x];
			returnStringNew.pushBack(newValue);
		}
		returnStringNew.resize(32);
		return returnStringNew;
	}

	ContIterator::String Randomizer::randomizeString(int64_t length) {
		ContIterator::String returnString{};
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		for (uint32_t x = 0; x < length; x++) {
			auto theValue = static_cast<char>((static_cast<float>(randomEngine()) / static_cast<float>(randomEngine.max()) * 93.0f) + 35.0f);
			if (static_cast<char>(theValue) == static_cast<char>(',') || static_cast<char>(theValue) == '\'' || static_cast<char>(theValue) == '/' ||
				static_cast<char>(theValue) == '\"' || static_cast<char>(theValue) == '\\' || static_cast<char>(theValue) == '`' || static_cast<char>(theValue) == '{' ||
				static_cast<char>(theValue) == '}' || static_cast<char>(theValue) == '[' || static_cast<char>(theValue) == ']' || static_cast<char>(theValue) == '(' ||
				static_cast<char>(theValue) == ')' || static_cast<char>(theValue) == static_cast<char>(0) || static_cast<char>(theValue) == static_cast<char>(1)) {
				theValue = static_cast<char>('s');
			}
			returnString.pushBack(theValue);
		}
		return returnString;
	}

	void Randomizer::randomizeId(Snowflake& theString, uint64_t minValue, uint64_t maxValue) {
		theString = ContIterator::String{ std::to_string(randomize64BitUInt(minValue, maxValue)) };
	}

	uint64_t Randomizer::randomize64BitUInt(double mean, double stdDeviation) {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		std::normal_distribution<> normalDistributionTwo{ mean, stdDeviation };
		auto theResult = normalDistributionTwo(randomEngine);
		if (theResult < 0) {
			theResult = -theResult;
		}
		return static_cast<uint64_t>(theResult);
	}

	uint64_t Randomizer::randomize64BitUInt(uint64_t minValue, uint64_t maxValue) {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		double difference = maxValue - minValue;
		uint64_t theValue = static_cast<uint64_t>(static_cast<double>(randomEngine()) / static_cast<double>(randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	uint32_t Randomizer::randomize32BitUInt(uint32_t minValue, uint32_t maxValue) {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		double difference = maxValue - minValue;
		uint32_t theValue = static_cast<uint32_t>(static_cast<double>(randomEngine()) / static_cast<double>(randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	uint16_t Randomizer::randomize16BitUInt(uint16_t minValue, uint16_t maxValue) {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		double difference = maxValue - minValue;
		uint16_t theValue = static_cast<uint16_t>(static_cast<double>(randomEngine()) / static_cast<double>(randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	uint8_t Randomizer::randomize8BitUInt(uint8_t minValue, uint8_t maxValue) {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		double difference = maxValue - minValue;
		uint8_t theValue  = static_cast<uint8_t>(static_cast<double>(randomEngine()) / static_cast<double>(randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	int8_t Randomizer::randomize8BitInt(uint8_t minValue, uint8_t maxValue) {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		double difference = maxValue - minValue;
		int8_t theValue	  = static_cast<int8_t>(static_cast<double>(randomEngine()) / static_cast<double>(randomEngine.max()) * difference) + minValue;
		return theValue;
	}

	uint64_t Randomizer::drawRandomValue(ContIterator::Vector<uint64_t>& theValues) {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		if (theValues.size() == 0) {
			return 0;
		}
		uint32_t valueIndex =
			static_cast<uint32_t>((static_cast<float>(randomEngine()) / static_cast<float>(randomEngine.max())) * static_cast<float>(theValues.size()));
		return theValues[valueIndex];
	}

	uint64_t Randomizer::randomize64BitUInt() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		return static_cast<uint64_t>(randomEngine());
	}

	uint32_t Randomizer::randomize32BitUInt() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		return static_cast<uint32_t>(randomEngine());
	}

	uint16_t Randomizer::randomize16BitUInt() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		return static_cast<uint16_t>(randomEngine());
	}

	double Randomizer::randomize64BitFloat() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		return static_cast<double>(randomEngine());
	}

	uint8_t Randomizer::randomize8BitUInt() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		auto theValue = static_cast<uint8_t>(randomEngine());
		if (static_cast<char>(theValue) == static_cast<char>(',') || static_cast<char>(theValue) == '\'' || static_cast<char>(theValue) == '/' ||
			static_cast<char>(theValue) == '"' || static_cast<char>(theValue) == '\\' || static_cast<char>(theValue) == '`' || static_cast<char>(theValue) == '{' ||
			static_cast<char>(theValue) == '}' || static_cast<char>(theValue) == '[' || static_cast<char>(theValue) == ']' || static_cast<char>(theValue) == '(' ||
			static_cast<char>(theValue) == ')' || static_cast<char>(theValue) == static_cast<char>(0) || static_cast<char>(theValue) == static_cast<char>(1)) {
			theValue = static_cast<char>('s');
		}
		return theValue;
	}

	int64_t Randomizer::randomize64BitInt() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		return static_cast<int64_t>(randomEngine());
	}

	int32_t Randomizer::randomize32BitInt() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		return static_cast<int32_t>(randomEngine());
	}

	int16_t Randomizer::randomize16BitInt() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		return static_cast<int16_t>(randomEngine());
	}

	float Randomizer::randomize32BitFloat() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		return static_cast<float>(randomEngine());
	}

	int8_t Randomizer::randomize8BitInt() {
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ randomDevice() };
		auto theValue = static_cast<int8_t>(randomEngine());
		if (static_cast<char>(theValue) == static_cast<char>(',') || static_cast<char>(theValue) == '\'' || static_cast<char>(theValue) == '/' ||
			static_cast<char>(theValue) == '"' || static_cast<char>(theValue) == '\\' || static_cast<char>(theValue) == '`' || static_cast<char>(theValue) == '{' ||
			static_cast<char>(theValue) == '}' || static_cast<char>(theValue) == '[' || static_cast<char>(theValue) == ']' || static_cast<char>(theValue) == '(' ||
			static_cast<char>(theValue) == ')' || static_cast<char>(theValue) == static_cast<char>(0) || static_cast<char>(theValue) == static_cast<char>(1)) {
			theValue = static_cast<char>('s');
		}
		return theValue;
	}

}