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
/// DiscordCoreClient01.cpp - Source file for the main/exposed DiscordCoreClient class.
/// May 22, 2022
/// https://github.com/RealTimeChris/discord_core_loader
/// \file DiscordCoreClient.cpp

#include <discordcoreloader/DiscordCoreClient.hpp>
#include <csignal>
#include <atomic>

namespace discord_core_loader {

	namespace Globals {
		std::atomic_bool doWeDisconnect{ false };
		std::atomic_bool doWeQuit{ false };
	}

	void atexitHandler() {
		Globals::doWeQuit.store(true);
	}

	SIGTERMError::SIGTERMError(const std::string& string) : std::runtime_error(string){};

	SIGSEGVError::SIGSEGVError(const std::string& string) : std::runtime_error(string){};

	SIGINTError::SIGINTError(const std::string& string) : std::runtime_error(string){};

	SIGILLError::SIGILLError(const std::string& string) : std::runtime_error(string){};

	SIGABRTError::SIGABRTError(const std::string& string) : std::runtime_error(string){};

	SIGFPEError::SIGFPEError(const std::string& string) : std::runtime_error(string){};

	void signalHandler(int32_t value, std::source_location location) {
		try {
			switch (value) {
				case SIGTERM: {
					throw SIGTERMError{ "Exiting for: SIGTERM.\n" };
				}
				case SIGSEGV: {
					throw SIGSEGVError{ "Exiting for: SIGSEGV.\n" };
				}
				case SIGINT: {
					throw SIGINTError{ "Exiting for: SIGINT.\n" };
				}
				case SIGILL: {
					throw SIGILLError{ "Exiting for: SIGILL.\n" };
				}
				case SIGABRT: {
					throw SIGABRTError{ "Exiting for: SIGABRT.\n" };
				}
				case SIGFPE: {
					throw SIGFPEError{ "Exiting for: SIGFPE.\n" };
				}
			}
		} catch (SIGINTError&) {
			reportException("signalHandler()");
			std::exit(EXIT_SUCCESS);
		} catch (...) {
			reportException("signalHandler()");
		}
		std::exit(EXIT_FAILURE);
	}

	DiscordCoreClient::DiscordCoreClient(const jsonifier::string& configFilePath) {
#ifdef _WIN32
		_crt_signal_t errorLambda = [](int32_t integer) -> void {
			signalHandler(integer);
		};
#else
		__sighandler_t errorLambda = [](int32_t integer) -> void {
			signalHandler(integer);
		};
#endif
		std::atexit(&atexitHandler);
		std::signal(SIGTERM, errorLambda);
		std::signal(SIGSEGV, errorLambda);
		std::signal(SIGINT, errorLambda);
		std::signal(SIGILL, errorLambda);
		std::signal(SIGABRT, errorLambda);
		std::signal(SIGFPE, errorLambda);
		this->configParser = ConfigParser{ configFilePath };
		this->guildQuantity.store(this->configParser.getTheData().guildQuantity);
		this->jsonifier = this->configParser.getTheData();
	}

	void DiscordCoreClient::initSocketAgents() {
		try {
			while (!this->connectionStopWatch.hasTimePassed()) {
				std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
			}
			uint64_t baseSocketAgentCount = configParser.getTheData().shardCount > std::thread::hardware_concurrency()
				? configParser.getTheData().shardCount / std::thread::hardware_concurrency()
				: configParser.getTheData().shardCount;

			for (uint64_t x = 0; x < baseSocketAgentCount; ++x) {
				this->baseSocketAgentMap[x] = std::make_unique<BaseSocketAgent>(this, &Globals::doWeQuit, true);
			}
			for (uint64_t x = 0; x < configParser.getTheData().shardCount; ++x) {
				this->baseSocketAgentMap[x % baseSocketAgentCount]->theClients[x] = std::make_unique<WebSocketSSLShard>(
					configParser.getTheData().doWePrintWebSocketErrorMessages, this->baseSocketAgentMap[x % baseSocketAgentCount].get());
			}
			
		} catch (...) {
			if (this->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::connect()");
			}
		}
	}

	void DiscordCoreClient::runServer() {
		initSocketAgents();
		while (!Globals::doWeQuit.load()) {
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
		}
		this->baseSocketAgentMap[this->shardingOptions.startingShard]->getTheTask()->join();
	}
}
