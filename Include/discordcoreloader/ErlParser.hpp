/*
	discord_core_loader, A bot library for Discord, written in C++, and featuring explicit multithreading through the usage of custom, asynchronous C++ CoRoutines.

	Copyright 2021, 2022 Chris M. (RealTimeChris)

	This file is part of DiscordCoreLoader.
	discord_core_loader is free software: you can redistribute it and/or modify it under the terms of the GNU
	General Public License as published by the Free Software Foundation, either version 3 of the License,
	or (at your option) any later version.
	discord_core_loader is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
	even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
	You should have received a copy of the GNU General Public License along with discord_core_loader.
	If not, see <https://www.gnu.org/licenses/>.
*/
/// ErlParser.hpp - Header for the erlpacking class.
/// Nov 8, 2021
/// https://github.com/RealTimeChris/discord_core_loader
/// \file ErlParser.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <array>

namespace discord_core_loader {

	struct ErlParseError : public std::runtime_error {
	  public:
		explicit ErlParseError(const jsonifier::string& message);
	};

	class ErlParser {
	  public:
		jsonifier::string& parseEtfToJson(jsonifier::string_view dataToParse);

	  protected:
		std::array<char, 1024 * 16> stringBuffer{};
		jsonifier::string_view dataBuffer{};
		jsonifier::string finalString{};
		uint64_t offSet{};

		template<typename RTy> RTy readBitsFromBuffer() {
			if (this->offSet + sizeof(RTy) > this->dataBuffer.size()) {
				throw ErlParseError{ "ErlParser::readBitsFromBuffer() Error: readBitsFromBuffer() past end of the buffer.\n\n" };
			}
			RTy newValue = *reinterpret_cast<const RTy*>(this->dataBuffer.data() + this->offSet);
			this->offSet += sizeof(RTy);
			reverseByteOrder<RTy>(newValue);
			return newValue;
		}

		void writeCharactersFromBuffer(uint32_t size);

		void writeCharacters(const char* data, std::size_t size);

		void writeCharacter(const char theChar);

		void singleValueETFToJson();

		void parseSmallIntegerExt();

		void parseSmallAtomExt();

		void parseNewFloatExt();

		void parseSmallBigExt();

		void parseIntegerExt();

		void parseStringExt();

		void parseBinaryExt();

		void parseListExt();

		void parseAtomExt();

		void parseNilExt();

		void parseMapExt();
	};
}// namespace DiscordCoreInternal
