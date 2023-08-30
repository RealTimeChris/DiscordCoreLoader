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
/// DiscordCoreClient01.cpp - Source file for the main/exposed DiscordCoreClient class.
/// May 22, 2022
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file DiscordCoreClient.cpp

#include <discordcoreloader/DiscordCoreClient.hpp>
#include <discordcoreloader/MessageHolder.hpp>
#include <csignal>
#include <atomic>

namespace DiscordCoreLoader {

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

	DiscordCoreClient::DiscordCoreClient(const std::string& configFilePath) : configParser{ configFilePath } {
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
		this->randomizer = this->configParser.getTheData();
		messageHolder	 = configParser.getTheData();
		std::cout << "Generating guild data..." << std::endl;
		messageHolder.generateGuildMessages(configParser.getTheData().guildQuantity);
		std::cout << "Done generating guild data - You can now attempt to connect your bot." << std::endl;
	}

	void DiscordCoreClient::connectShard() {
		try {
			while (!this->connectionStopWatch.hasTimePassed()) {
				std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
			}
			if (!this->baseSocketAgentMap.contains(0)) {
				this->baseSocketAgentMap[0] = std::make_unique<BaseSocketAgent>(this->webSocketSSLServerMain.get(), this, &Globals::doWeQuit, true);
			}
			this->connectionStopWatch.resetTimer();
			auto newShard = std::make_unique<WebSocketSSLShard>(this->webSocketSSLServerMain->getNewSocket(), this->webSocketSSLServerMain->context,
				this->configParser.getTheData().doWePrintWebSocketErrorMessages, this->baseSocketAgentMap[0].get());

			ContIterator::Vector<WebSocketSSLShard*> theVector{};
			theVector.emplace_back(newShard.get());
			while (newShard->authKey == "") {
				this->webSocketSSLServerMain->processIO(theVector);
				this->baseSocketAgentMap[0]->handleBuffer(newShard.get());
			}
			newShard->shard[0] = -1;
			std::string sendString{ "HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + newShard->authKey + "\r\n\r\n" };
			this->baseSocketAgentMap[0]->sendMessage(&sendString, newShard.get(), false);
			auto returnValue = this->webSocketSSLServerMain->processIO(theVector);
			this->baseSocketAgentMap[0]->sendHelloMessage(newShard.get());
			returnValue = this->webSocketSSLServerMain->processIO(theVector);
			theVector.clear();
			theVector.emplace_back(newShard.get());
			while (newShard->shard[0] == -1) {
				returnValue = this->webSocketSSLServerMain->processIO(theVector);
				this->baseSocketAgentMap[0]->handleBuffer(newShard.get());
			}
			auto theCurrentShard		   = newShard->shard[0];
			auto theCurrentBaseSocketAgent = newShard->shard[0] % this->workerCount;
			if (!this->baseSocketAgentMap.contains(theCurrentBaseSocketAgent)) {
				this->baseSocketAgentMap[theCurrentBaseSocketAgent] = std::make_unique<BaseSocketAgent>(this->webSocketSSLServerMain.get(), this, &Globals::doWeQuit, true);
			}
			if (this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients.contains(theCurrentShard)) {
				this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard]->disconnect();
			}
			this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard] = std::move(newShard);

			this->baseSocketAgentMap[theCurrentBaseSocketAgent]->sendFinalMessage(this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard].get());
			while (!this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard]->areWeConnected) {
				returnValue = this->webSocketSSLServerMain->processIO(theVector);
				this->baseSocketAgentMap[theCurrentBaseSocketAgent]->handleBuffer(this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard].get());
			}
			this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard]->theAgent	 = this->baseSocketAgentMap[theCurrentBaseSocketAgent].get();
			this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard]->sendGuilds = true;
			if (this->configParser.getTheData().doWePrintGeneralSuccessMessages) {
				std::cout << shiftToBrightGreen()
						  << "Connected Shard " +

						std::to_string(this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard]->shard[0])
						  << " of " << this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard]->shard[1]
						  << std::string(" Shards for this process. (") +
						std::to_string(this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard]->shard[0]) + " of " +
						std::to_string(this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard]->shard[1]) +
						std::string(" Shards total across all processes)")
						  << reset() << std::endl;
			}
			if (this->baseSocketAgentMap[theCurrentBaseSocketAgent]->theClients[theCurrentShard]->shard[0] == this->totalShardCount.load() - 1) {
				std::cout << shiftToBrightGreen() << "All of the shards are connected for the current process!" << reset() << std::endl << std::endl;
			}
		} catch (...) {
			if (this->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::connect()");
			}
		}
	}

	void DiscordCoreClient::runServer() {
		this->webSocketSSLServerMain = std::make_unique<WebSocketSSLServerMain>(this->configParser.getTheData().connectionIp, this->configParser.getTheData().connectionPort, true,
			&Globals::doWeQuit, &this->configParser);
		while (!Globals::doWeQuit.load()) {
			this->connectShard();
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
		}
		this->baseSocketAgentMap[this->shardingOptions.startingShard]->getTheTask()->join();
	}
}
