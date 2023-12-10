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
/// Main.cpp - Defines the entry point for the application.
/// May 22, 2022
/// https://github.com/RealTimeChris/discord_core_loader
/// \file Main.cpp

#include <discordcoreloader/DiscordCoreClient.hpp>

int32_t main() {
	try {
		discord_core_loader::DiscordCoreClient theClient{ "Config.json" };
		theClient.runServer();
	} catch (...) {
		discord_core_loader::reportException("main()");
	}
	return 0;
}