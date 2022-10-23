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
/// JSONIfier.hpp - Header file for the Jsonifier stuff.
/// May 22, 2022
/// https://discordcoreapi.com
/// \file JSONIfier.hpp

#pragma once

#include <discordcoreloader/ErlParser.hpp>
#include <discordcoreloader/ObjectGenerator.hpp>
#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/Randomizer.hpp>

namespace DiscordCoreLoader {

	class JSONIFier : public ObjectGenerator {
	  public:
		JSONIFier() = default;

		JSONIFier& operator=(JSONIFier&) noexcept = default;

		JSONIFier(JSONIFier&) noexcept = default;

		JSONIFier& operator=(ConfigData&&);

		JSONIFier(ConfigData&& configData);

		Jsonifier JSONIFYUnavailableGuild(UnavailableGuild& theGuild);

		Jsonifier JSONIFYGuildMember(GuildMemberData&&);

		Jsonifier JSONIFYGuild(GuildData&& guildOld);

		Jsonifier JSONIFYChannel(ChannelData&&);

		Jsonifier JSONIFYRole(RoleData&&);

		Jsonifier JSONIFYUser(UserData&&);
	};

}