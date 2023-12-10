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
#include <discordcoreloader/DataParsingFunctions.hpp>
#include <filesystem>

namespace DiscordCoreLoader {

	ConfigParser::ConfigParser(std::string configFilePath) {
		this->parseConfigData(configFilePath);
	};

	ConfigData ConfigParser::getTheData() {
		return this->theData;
	}

	void ConfigParser::parseConfigData(std::string configFilePath) {
		std::stringstream theStream{};
		theStream << std::filesystem::current_path();
		std::string currentPath{ theStream.str().substr(1, theStream.str().size() - 2) };
#ifdef _WIN32
		currentPath += "\\" + configFilePath;
#elif __linux__
		currentPath += "/" + configFilePath;
#endif
		simdjson::ondemand::parser parser{};
		std::string fileContents = loadFileContents(currentPath);
		fileContents.reserve(fileContents.size() + simdjson::SIMDJSON_PADDING);
		auto theDocument = parser.iterate(fileContents.data(), fileContents.length(), parser.capacity());
		this->theData.connectionIp = theDocument["ConnectionIp"].get_string().take_value();
		this->theData.connectionPort = theDocument["ConnectionPort"].get_string().take_value();
		this->theData.doWePrintGeneralSuccessMessages = theDocument["DoWePrintGeneralSuccessMessages"].get_bool().take_value();
		this->theData.doWePrintGeneralErrorMessages = theDocument["DoWePrintGeneralErrorMessages"].get_bool().take_value();
		this->theData.doWePrintWebSocketSuccessReceiveMessages = theDocument["DoWePrintWebSocketSuccessReceiveMessages"].get_bool().take_value();
		this->theData.doWePrintWebSocketSuccessSentMessages = theDocument["DoWePrintWebSocketSuccessSentMessages"].get_bool().take_value();
		this->theData.doWePrintWebSocketErrorMessages = theDocument["DoWePrintWebSocketErrorMessages"].get_bool().take_value();
		this->theData.guildQuantity = theDocument["GuildQuantity"].get_uint64().take_value();
		this->theData.stdDeviationForStringLength = theDocument["StdDeviationForStringLength"].get_uint64().take_value();
		this->theData.meanForStringLength = theDocument["MeanForStringLength"].get_uint64().take_value();
		this->theData.stdDeviationForRoleCount = theDocument["StdDeviationForRoleCount"].get_uint64().take_value();
		this->theData.meanForRoleCount = theDocument["MeanForRoleCount"].get_uint64().take_value();
		this->theData.stdDeviationForChannelCount = theDocument["StdDeviationForChannelCount"].get_uint64().take_value();
		this->theData.meanForChannelCount = theDocument["MeanForChannelCount"].get_uint64().take_value();
		this->theData.stdDeviationForMemberCount = theDocument["StdDeviationForMemberCount"].get_uint64().take_value();
		this->theData.meanForMemberCount = theDocument["MeanForMemberCount"].get_uint64().take_value();
		this->theData.stdDeviationForRoleCount = theDocument["StdDeviationForRoleCount"].get_uint64().take_value();
		this->theData.meanForRoleCount = theDocument["MeanForRoleCount"].get_uint64().take_value();
	}

}// namespace DiscordCoreLoader
