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
/// main.cpp - Defines the entry point for the application.
/// May 22, 2022
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file main.cpp

#include <discordcoreloader/DiscordCoreClient.hpp>

int32_t main() {
	try {
		std::cout << "WERE HERE THIS IS IT!" << std::endl;
		DiscordCoreLoader::DiscordCoreClient theClient{ "Config.json" };
		theClient.runServer();
	} catch (...) {
		DiscordCoreLoader::reportException("main()");
	}
	return 0;
}