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
#include <discordcoreloader/Randomizer.hpp>
#include <discordcoreloader/ErlParser.hpp>
#include <jsonifier/Index.hpp>

namespace DiscordCoreLoader {

	inline thread_local Jsonifier::JsonifierCore parser{};

	inline thread_local EtfSerializer serializer{};

	class MessageHolder {
	  public:

		inline MessageHolder(){};

		void generateGuildMessagesFinal(uint64_t guildCount);
		
		void generateGuildMessages(uint64_t guildCount);
		
		bool collectNextMessage(ContIterator::String& newMessage);
		
	  protected:
		ContIterator::Vector<std::unique_ptr<std::jthread>> jthreads{};
		std::deque<ContIterator::String> messagesToSend{};
		std::atomic_uint32_t lastNumberSent{};
		std::mutex accessMutex{};
	};
}