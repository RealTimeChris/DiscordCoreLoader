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
/// https://discordcoreapi.com
/// \file ConfigParser.cpp

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/ConfigParser.hpp>
#include <filesystem>

namespace DiscordCoreLoader {

	ConfigParser::ConfigParser(std::string configFilePath) {
		std::stringstream theStream{};
		theStream << std::filesystem::current_path();
		std::string currentPath{ theStream.str().substr(1, theStream.str().size() - 2) };
#ifdef WIN32
		currentPath += "\\" + configFilePath;
#else
		currentPath += "/" + configFilePath;
#endif
		this->configFile = nlohmann::json::parse(loadFileContents(currentPath));
		this->parseConfigData();
	};

	ConfigData ConfigParser::getTheData() {
		return this->theData;
	}

	void ConfigParser::parseConfigData() {
		if (this->configFile.contains("ConnectionIp") && !this->configFile["ConnectionIp"].is_null()) {
			this->theData.connectionIp = this->configFile["ConnectionIp"].get<std::string>();
		}
		if (this->configFile.contains("ConnectionPort") && !this->configFile["ConnectionPort"].is_null()) {
			this->theData.connectionPort = this->configFile["ConnectionPort"].get<std::string>();
		}
		if (this->configFile.contains("DoWePrintGeneralSuccessMessages") && !this->configFile["DoWePrintGeneralSuccessMessages"].is_null()) {
			this->theData.doWePrintGeneralSuccessMessages = this->configFile["DoWePrintGeneralSuccessMessages"].get<bool>();
		}
		if (this->configFile.contains("DoWePrintGeneralErrorMessages") && !this->configFile["DoWePrintGeneralErrorMessages"].is_null()) {
			this->theData.doWePrintGeneralErrorMessages = this->configFile["DoWePrintGeneralErrorMessages"].get<bool>();
		}
		if (this->configFile.contains("DoWePrintWebSocketSuccessSentMessages") && !this->configFile["DoWePrintWebSocketSuccessSentMessages"].is_null()) {
			this->theData.doWePrintWebSocketSuccessSentMessages = this->configFile["DoWePrintWebSocketSuccessSentMessages"].get<bool>();
		}
		if (this->configFile.contains("DoWePrintWebSocketSuccessReceiveMessages") && !this->configFile["DoWePrintWebSocketSuccessReceiveMessages"].is_null()) {
			this->theData.doWePrintWebSocketSuccessReceiveMessages = this->configFile["DoWePrintWebSocketSuccessReceiveMessages"].get<bool>();
		}
		if (this->configFile.contains("DoWePrintWebSocketErrorMessages") && !this->configFile["DoWePrintWebSocketErrorMessages"].is_null()) {
			this->theData.doWePrintWebSocketErrorMessages = this->configFile["DoWePrintWebSocketErrorMessages"].get<bool>();
		}
		if (this->configFile.contains("GuildQuantity") && !this->configFile["GuildQuantity"].is_null()) {
			this->theData.guildQuantity = this->configFile["GuildQuantity"].get<uint64_t>();
		}
		if (this->configFile.contains("StdDeviationForStringLength") && !this->configFile["StdDeviationForStringLength"].is_null()) {
			this->theData.stdDeviationForStringLength = this->configFile["StdDeviationForStringLength"].get<uint64_t>();
		}
		if (this->configFile.contains("MeanForStringLength") && !this->configFile["MeanForStringLength"].is_null()) {
			this->theData.meanForStringLength = this->configFile["MeanForStringLength"].get<uint64_t>();
		}
		if (this->configFile.contains("StdDeviationForMemberCount") && !this->configFile["StdDeviationForMemberCount"].is_null()) {
			this->theData.stdDeviationForMemberCount = this->configFile["StdDeviationForMemberCount"].get<uint64_t>();
		}
		if (this->configFile.contains("MeanForMemberCount") && !this->configFile["MeanForMemberCount"].is_null()) {
			this->theData.meanForMemberCount = this->configFile["MeanForMemberCount"].get<uint64_t>();
		}
		if (this->configFile.contains("StdDeviationForChannelCount") && !this->configFile["StdDeviationForChannelCount"].is_null()) {
			this->theData.stdDeviationForChannelCount = this->configFile["StdDeviationForChannelCount"].get<uint64_t>();
		}
		if (this->configFile.contains("MeanForChannelCount") && !this->configFile["MeanForChannelCount"].is_null()) {
			this->theData.meanForChannelCount = this->configFile["MeanForChannelCount"].get<uint64_t>();
		}
		if (this->configFile.contains("StdDeviationForRoleCount") && !this->configFile["StdDeviationForRoleCount"].is_null()) {
			this->theData.stdDeviationForRoleCount = this->configFile["StdDeviationForRoleCount"].get<uint64_t>();
		}
		if (this->configFile.contains("MeanForRoleCount") && !this->configFile["MeanForRoleCount"].is_null()) {
			this->theData.meanForRoleCount = this->configFile["MeanForRoleCount"].get<uint64_t>();
		}
	}

}// namespace DiscordCoreLoader
