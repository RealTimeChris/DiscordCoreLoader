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
/// MessageHolderhpp - Header file for the MessageHolder stuff.
/// Aug 27, 2023
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file MessageHolder.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/ObjectGenerator.hpp>
#include <discordcoreloader/JsonSpecializations.hpp>
#include <discordcoreloader/DiscordCoreClient.hpp>
#include <discordcoreloader/Randomizer.hpp>
#include <discordcoreloader/ErlParser.hpp>
#include <jsonifier/Index.hpp>

namespace DiscordCoreLoader {

	void MessageHolder ::generateGuildMessagesFinal(uint64_t guildCount) {
		for (size_t x = 0; x < guildCount; ++x) {
			if (messagesToSend.size() % 100 == 0) {
				std::cout << "Generating guild #" << messagesToSend.size() + 1 << "'s data." << std::endl;
			}
			Snowflake newId{};
			ObjectGenerator::randomizeId(newId, 0);
			auto newGuild = ObjectGenerator::generateGuild(newId);
			if (DiscordCoreClient::configData.format == "Json") {
				ContIterator::String stringBufferNew{};
				WebSocketMessageReal<GuildData> data{};
				data.d = std::move(newGuild);
				data.s = lastNumberSent.load();
				data.t = "GUILD_CREATE";
				parser.serializeJson(data, stringBufferNew);
				std::unique_lock lock{ accessMutex };
				messagesToSend.emplace_back(std::move(stringBufferNew));
			} else {
				EtfSerializer guild{ newGuild };
				EtfSerializer serializer{};
				serializer["d"] = std::move(guild);
				serializer["s"] = lastNumberSent.load();
				serializer["t"] = "GUILD_CREATE";
				std::unique_lock lock{ accessMutex };
				messagesToSend.emplace_back(std::move(serializer.operator ContIterator::String()));
			}
			std::atomic_fetch_add(&lastNumberSent, 1);
		}
	}

	void MessageHolder ::generateGuildMessages(uint64_t guildCount) {
		jthreads.resize(std::jthread::hardware_concurrency());
		for (size_t x = 0; x < std::jthread::hardware_concurrency(); ++x) {
			jthreads[x] = std::make_unique<std::jthread>(std::jthread{ [=]() {
				generateGuildMessagesFinal(guildCount / std::jthread::hardware_concurrency());
			} });
		}
	}

	bool MessageHolder ::collectNextMessage(ContIterator::String& newMessage) {
		std::unique_lock lock{ accessMutex };
		if (messagesToSend.size() > 0) {
			newMessage = std::move(messagesToSend.front());
			messagesToSend.pop_front();
			return true;
		}

		return false;
	}
}