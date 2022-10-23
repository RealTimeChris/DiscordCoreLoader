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
/// Randomizer.hpp - Header file for the Randomizer stuff.
/// May 22, 2022
/// https://discordcoreapi.com
/// \file Randomizer.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <charconv>

namespace DiscordCoreLoader {

	class Randomizer {
	  public:
		void randomizeId(std::string& theString, uint64_t minValue = 0, uint64_t maxValue = UINT64_MAX);

		uint64_t randomize64BitUInt(uint64_t minValue, uint64_t maxValue);

		uint32_t randomize32BitUInt(uint32_t minValue, uint32_t maxValue);

		uint16_t randomize16BitUInt(uint16_t minValue, uint16_t maxValue);

		uint8_t randomize8BitUInt(uint8_t minValue, uint8_t maxValue);

		uint64_t randomize64BitUInt(double mean, double stdDeviation);

		int8_t randomize8BitInt(uint8_t minValue, uint8_t maxValue);

		uint64_t drawRandomValue(std::vector<uint64_t>&);

		std::string randomizeString(int64_t length);

		std::string randomizeIconHash();

		uint64_t randomize64BitUInt();

		uint32_t randomize32BitUInt();

		uint16_t randomize16BitUInt();

		double randomize64BitFloat();

		uint8_t randomize8BitUInt();

		int64_t randomize64BitInt();

		int32_t randomize32BitInt();

		int16_t randomize16BitInt();

		float randomize32BitFloat();

		int8_t randomize8BitInt();

	  protected:
		std::random_device randomDevice{};
		std::mt19937_64 randomEngine{ this->randomDevice() };
		std::recursive_mutex theMutex{};
	};
}