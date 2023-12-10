/*
*
	discord_core_loader, A stress-tester for Discord bot libraries, and Discord bots.

	Copyright 2022 Chris M. (RealTimeChris)

	This file is part of DiscordCoreLoader.
	discord_core_loader is free software: you can redistribute it and/or modify it under the terms of the GNU
	General Public License as published by the Free Software Foundation, either version 3 of the License,
	or (at your option) any later version.
	discord_core_loader is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
	even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
	You should have received a copy of the GNU General Public License along with discord_core_loader.
	If not, see <https://www.gnu.org/licenses/>.

*/
/// ConfigParser.cpp - Source file for the config parser class.
/// May 22, 2022
/// https://github.com/RealTimeChris/discord_core_loader
/// \file ConfigParser.cpp

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/JsonSpecializations.hpp>
#include <discordcoreloader/ConfigParser.hpp>
#include <filesystem>

namespace discord_core_loader {

	ConfigParser::ConfigParser(jsonifier::string configFilePath) {
		this->parseConfigData(configFilePath);
	};

	ConfigData& ConfigParser::getTheData() {
		return this->theData;
	}

	void ConfigParser::parseConfigData(jsonifier::string configFilePath) {
		std::stringstream theStream{};
		theStream << std::filesystem::current_path();
		jsonifier::string currentPath{ theStream.str().substr(1, theStream.str().size() - 2) };
#ifdef _WIN32
		currentPath += "\\" + configFilePath;
#elif __linux__
		currentPath += "/" + configFilePath;
#endif
		jsonifier::string fileContents = loadFileContents(currentPath);
		parser.parseJson(this->theData, fileContents);
	}

}// namespace discord_core_loader
