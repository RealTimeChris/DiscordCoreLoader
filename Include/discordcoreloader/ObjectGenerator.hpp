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
/// ObjectGenerator.hpp - Header file for the ObjectGenerator stuff.
/// May 22, 2022
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file ObjectGenerator.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/ConfigParser.hpp>
#include <discordcoreloader/Randomizer.hpp>

namespace DiscordCoreLoader {

	class ObjectGenerator : public Randomizer {
	  public:
		ObjectGenerator() noexcept = default;

		static GuildMemberData generateGuildMember(const ContIterator::String& guildId, const ContIterator::Vector<RoleData>& roles);

		static UnavailableGuild generateUnavailableGuild(uint64_t minValue, uint64_t maxValue);

		static ChannelData generateChannel(Snowflake guildId);

		static GuildData generateGuild(Snowflake guildId);

		static RoleData generateRole(uint64_t position);

		static UserData generateUser();
	};
}