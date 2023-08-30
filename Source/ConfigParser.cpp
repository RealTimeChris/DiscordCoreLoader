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
/// ConfigParser.cpp - Source file for the config parser class.
/// May 22, 2022
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file ConfigParser.cpp

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/ConfigParser.hpp>
#include <discordcoreloader/JsonSpecializations.hpp>
#include <filesystem>

namespace DiscordCoreLoader {

	ConfigParser::ConfigParser(Jsonifier::String configFilePath) {
		this->parseConfigData(configFilePath);
	};

	ConfigData ConfigParser::getTheData() {
		return this->theData;
	}

	void ConfigParser::parseConfigData(Jsonifier::String configFilePath) {
		std::stringstream theStream{};
		theStream << std::filesystem::current_path();
		Jsonifier::String currentPath{ theStream.str().substr(1, theStream.str().size() - 2) };

#ifdef _WIN32
		currentPath += "\\" + configFilePath;
#elif __linux__
		currentPath += "/" + configFilePath;
#endif
		Jsonifier::JsonifierCore parser{};
		Jsonifier::String fileContents = loadFileContents(currentPath);
		parser.parseJson<true, true>(this->theData, fileContents);
	}

}// namespace DiscordCoreLoader
