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
/// ConfigParser.hpp - Header file for the config parser class.
/// May 22, 2022
/// https://github.com/RealTimeChris/discord_core_loader
/// \file ConfigParser.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>

namespace discord_core_loader {

	enum class TextFormat {
		ETF = 0,///< Erlang text format.
		JSON = 1///< Json text format.
	};

	struct ConfigData {
		jsonifier::string connectionIp{};
		jsonifier::string connectionPort{};
		bool doWePrintWebSocketSuccessReceiveMessages{ false };
		bool doWePrintWebSocketSuccessSentMessages{ false };
		bool doWePrintWebSocketErrorMessages{ false };
		bool doWePrintGeneralSuccessMessages{ false };
		bool doWePrintGeneralErrorMessages{ false };
		uint64_t shardCount{ 1 };
		uint64_t guildQuantity{ 0 };
		uint64_t stdDeviationForStringLength{};
		uint64_t meanForStringLength{};
		uint64_t stdDeviationForMemberCount{};
		uint64_t meanForMemberCount{};
		uint64_t stdDeviationForChannelCount{};
		uint64_t meanForChannelCount{};
		uint64_t stdDeviationForRoleCount{};
		uint64_t meanForRoleCount{};
	};

	class ConfigParser {
	  public:
		ConfigParser(jsonifier::string theConfigPath);

		ConfigParser() = default;

		ConfigData& getTheData();

	  protected:
		ConfigData theData{};

		void parseConfigData(jsonifier::string configFilePath);
	};
}// namespace discord_core_loader
