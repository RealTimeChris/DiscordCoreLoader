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
/// DiscordCoreClient01.hpp - Header file for the main/exposed DiscordCoreClient class.
/// May 22, 2022
/// https://discordcoreapi.com
/// \file DiscordCoreClient.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/ConfigParser.hpp>
#include <discordcoreloader/SSLClients.hpp>
#include <discordcoreloader/WebSocketEntities.hpp>

namespace DiscordCoreLoader {

	extern "C" void signalHandler(int32_t);

	void atexitHandler();

	/**
	 * \addtogroup main_endpoints
	 * @{
	 */
	/// DiscordCoreClient - The main class for this library. \brief DiscordCoreClient - The main class for this library.
	class DiscordCoreClient {
	  public:
		friend class BaseSocketAgent;

		/// DiscordCoreClient constructor. \brief DiscordCoreClient constructor.
		/// \param configFilePath A string containing the path to the config file.
		DiscordCoreClient(const std::string& configFilePath);

		/// Executes the library, and waits for completion. \brief Executes the library, and waits for completion.
		void runServer();

		~DiscordCoreClient();

	  protected:
		std::unordered_map<std::string, std::unique_ptr<BaseSocketAgent>> baseSocketAgentMap{};
		StopWatch<std::chrono::milliseconds> theStopWatch{ std::chrono::milliseconds{ 5000 } };
		std::unique_ptr<WebSocketSSLServerMain> webSocketSSLServerMain{ nullptr };
		bool haveWeCollectedShardingInfo{ false };
		ShardingOptions shardingOptions{};
		nlohmann::json theGuildHolder{};
		ConfigParser configParser{};
#ifdef _WIN32
		WSADataWrapper theWSAData{};
#endif
		int32_t guildQuantity{};
		std::mutex coutMutex{};
		JSONIFier jsonifier{};
		int32_t workerCount{};


		void instantiateWebSockets();

		void generateGuildData();

		void collectShardInfo();
	};
	/**@}*/
}// namespace DiscordCoreLoader
