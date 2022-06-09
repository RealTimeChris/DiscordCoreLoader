/*
*
	Copyright 2021, 2022 Chris M.

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
/// https://discordcoreapi.com
/// \file DiscordCoreClient.cpp

#include <discordcoreloader/DiscordCoreClient.hpp>
#include <signal.h>
#include <atomic>

namespace DiscordCoreLoader {

	namespace Globals {
		std::atomic_bool doWeQuit{ false };
	}

	void atexitHandler() {
		Globals::doWeQuit.store(true);
	}

	void signalHandler(int32_t theEvent) {
		switch (theEvent) {
			case SIGABRT: {
				std::cout << "SIGABRT" << std::endl;
				break;
			}
			case SIGINT: {
				std::cout << "SIGINT" << std::endl;
				break;
			}
			case SIGILL: {
				std::cout << "SIGILL" << std::endl;
				break;
			}
			case SIGFPE: {
				std::cout << "SIGFPE" << std::endl;
				break;
			}
			case SIGSEGV: {
				std::cout << "SIGSEGV" << std::endl;
				break;
			}
			case SIGTERM: {
				std::cout << "SIGTERM" << std::endl;
				break;
			}
		}
	}

	DiscordCoreClient::DiscordCoreClient(const std::string& configFilePath) {
		std::atexit(atexitHandler);
		signal(SIGABRT, signalHandler);
		signal(SIGINT, signalHandler);
		signal(SIGILL, signalHandler);
		signal(SIGFPE, signalHandler);
		signal(SIGSEGV, signalHandler);
		signal(SIGTERM, signalHandler);
		this->configParser = ConfigParser{ configFilePath };
		this->guildQuantity = this->configParser.getTheData().guildQuantity;
		this->jsonifier = this->configParser.getTheData();
	}

	void DiscordCoreClient::runServer() {
		this->generateGuildData();
		this->collectShardInfo();
		this->instantiateWebSockets();
		this->baseSocketAgentMap[std::to_string(this->shardingOptions.startingShard)]->getTheTask()->join();
	}

	void DiscordCoreClient::collectShardInfo() {
		this->webSocketSSLServerMain =
			std::make_unique<WebSocketSSLServerMain>(this->configParser.getTheData().connectionIp, this->configParser.getTheData().connectionPort, true, &Globals::doWeQuit);
		auto thePtr = std::make_unique<DiscordCoreLoader::BaseSocketAgent>(this->webSocketSSLServerMain.get(), this, &Globals::doWeQuit);
		this->baseSocketAgentMap.insert_or_assign(std::to_string(0), std::move(thePtr));
		this->baseSocketAgentMap[std::to_string(0)]->connect(0, 0);
		this->baseSocketAgentMap[std::to_string(0)]->storeMessage(this->theGuildJson, this->baseSocketAgentMap[std::to_string(0)]->opCode);
		while (!this->haveWeCollectedShardingInfo) {
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
		}
	}

	void DiscordCoreClient::generateGuildData() {
		this->theGuildJson = this->jsonifier.JSONIFYGuild(this->jsonifier.generateGuild(this->jsonifier.randomizeId()));
	}

	void DiscordCoreClient::instantiateWebSockets() {
		this->shardingOptions.numberOfShardsForThisProcess = this->shardingOptions.totalNumberOfShards;
		int32_t workerCount{ static_cast<int32_t>(std::thread::hardware_concurrency()) - 1 };
		int32_t shardsPerWorker{ static_cast<int32_t>(floor(static_cast<float>(this->shardingOptions.totalNumberOfShards) / static_cast<float>(workerCount))) };
		int32_t leftOverShards{ this->shardingOptions.totalNumberOfShards - shardsPerWorker * workerCount };

		std::vector<int32_t> shardsPerWorkerVect{};
		for (int32_t x = 0; x < workerCount; x += 1) {
			int32_t newShardAmount{};
			shardsPerWorkerVect.push_back(shardsPerWorker);
			if (leftOverShards == 0) {
				continue;
			}
			newShardAmount = static_cast<int32_t>(ceil(static_cast<float>(leftOverShards) / static_cast<float>(std::thread::hardware_concurrency())));
			shardsPerWorkerVect[x] += newShardAmount;

			if (x == static_cast<int32_t>(std::thread::hardware_concurrency()) - 1) {
				shardsPerWorkerVect[0] += leftOverShards;
			}
			leftOverShards -= newShardAmount;
		}
		for (auto& value: shardsPerWorkerVect) {
			std::cout << "WORKER COUNT: " << value << std::endl;
		}
		int32_t totalShards{ 1 };
		for (int32_t y = 1; y < shardsPerWorkerVect[0]; y += 1) {
			totalShards += 1;
			//this->baseSocketAgentMap[std::to_string(0)]->connect(totalShards - 1, this->shardingOptions.totalNumberOfShards);
		}
		std::cout << "WORKER COUNT: 0202" << std::endl;
		for (int32_t x = 1; x < workerCount; x += 1) {
			auto thePtr02 = std::make_unique<DiscordCoreLoader::BaseSocketAgent>(this->webSocketSSLServerMain.get(), this, &Globals::doWeQuit);
			for (int32_t y = 0; y < shardsPerWorkerVect[x]; y += 1) {
				std::cout << "WORKER COUNT: 060606" << std::endl;
				totalShards += 1;
				thePtr02->connect(totalShards - 1, this->shardingOptions.totalNumberOfShards);
				std::cout << "WORKER COUNT: 0303" << std::endl;
				thePtr02->storeMessage(this->theGuildJson, thePtr02->opCode);
				std::cout << "WORKER COUNT: 0404" << std::endl;
			}
			this->baseSocketAgentMap.insert_or_assign(std::to_string(x), std::move(thePtr02));
		}
		if (this->configParser.getTheData().doWePrintGeneralSuccessMessages) {
			std::cout << shiftToBrightGreen() << "All of the shards are connected for the current process!" << reset() << std::endl << std::endl;
		}
	}

	DiscordCoreClient::~DiscordCoreClient() {
	}
}
