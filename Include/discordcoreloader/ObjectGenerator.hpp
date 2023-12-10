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
/// ObjectGenerator.hpp - Header file for the ObjectGenerator stuff.
/// May 22, 2022
/// https://github.com/RealTimeChris/discord_core_loader
/// \file ObjectGenerator.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/Randomizer.hpp>
#include <discordcoreloader/ConfigParser.hpp>

namespace discord_core_loader {

	class ObjectGenerator : public Randomizer {
	  public:
		std::unique_ptr<GuildMemberData> generateGuildMember(const jsonifier::string& guildId, const jsonifier::vector<std::unique_ptr<RoleData>>& roles);

		UnavailableGuild generateUnavailableGuild(uint64_t minValue, uint64_t maxValue);

		std::unique_ptr<ChannelData> generateChannel(jsonifier::string guildId);

		std::unique_ptr<GuildData> generateGuild(jsonifier::string guildId);

		std::unique_ptr<RoleData> generateRole(uint64_t position);

		std::unique_ptr<UserData> generateUser();

	  protected:
		uint64_t stdDeviationForStringLength{};
		uint64_t meanForStringLength{};
		uint64_t stdDeviationForMemberCount{};
		uint64_t meanForMemberCount{};
		uint64_t stdDeviationForChannelCount{};
		uint64_t meanForChannelCount{};
		uint64_t stdDeviationForRoleCount{};
		uint64_t meanForRoleCount{};
	};
}