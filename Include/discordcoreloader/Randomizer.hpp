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
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file Randomizer.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <charconv>

namespace DiscordCoreLoader {

	class Randomizer {
	  public:
		static void randomizeId(Snowflake& theString, uint64_t minValue = 0, uint64_t maxValue = UINT64_MAX);

		static uint64_t randomize64BitUInt(uint64_t minValue, uint64_t maxValue);

		static uint32_t randomize32BitUInt(uint32_t minValue, uint32_t maxValue);

		static uint16_t randomize16BitUInt(uint16_t minValue, uint16_t maxValue);

		static uint8_t randomize8BitUInt(uint8_t minValue, uint8_t maxValue);

		static uint64_t randomize64BitUInt(double mean, double stdDeviation);

		static int8_t randomize8BitInt(uint8_t minValue, uint8_t maxValue);

		static uint64_t drawRandomValue(ContIterator::Vector<uint64_t>&);

		static ContIterator::String randomizeString(int64_t length);

		static ContIterator::String randomizeIconHash();

		static uint64_t randomize64BitUInt();

		static uint32_t randomize32BitUInt();

		static uint16_t randomize16BitUInt();

		static double randomize64BitFloat();

		static uint8_t randomize8BitUInt();

		static int64_t randomize64BitInt();

		static int32_t randomize32BitInt();

		static int16_t randomize16BitInt();

		static float randomize32BitFloat();

		static int8_t randomize8BitInt();
	};
}