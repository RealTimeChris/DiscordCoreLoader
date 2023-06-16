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
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file DiscordCoreClient.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/ConfigParser.hpp>
#include <discordcoreloader/SSLClients.hpp>
#include <discordcoreloader/WebSocketEntities.hpp>
#include <source_location>

namespace DiscordCoreLoader {

	class SIGTERMError : public std::runtime_error {
	  public:
		SIGTERMError(const std::string& string);
	};

	class SIGSEGVError : public std::runtime_error {
	  public:
		SIGSEGVError(const std::string& string);
	};

	class SIGINTError : public std::runtime_error {
	  public:
		SIGINTError(const std::string& string);
	};

	class SIGILLError : public std::runtime_error {
	  public:
		SIGILLError(const std::string& string);
	};

	class SIGABRTError : public std::runtime_error {
	  public:
		SIGABRTError(const std::string& string);
	};

	class SIGFPEError : public std::runtime_error {
	  public:
		SIGFPEError(const std::string& string);
	};

	extern "C" void signalHandler(int32_t value, std::source_location location = std::source_location::current());

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

	  protected:
		StopWatch<std::chrono::milliseconds> connectionStopWatch{ std::chrono::milliseconds{ 5000 } };
		std::unordered_map<int32_t, std::unique_ptr<BaseSocketAgent>> baseSocketAgentMap{};
		std::unique_ptr<WebSocketSSLServerMain> webSocketSSLServerMain{ nullptr };
		bool haveWeCollectedShardingInfo{ false };
		std::atomic_uint32_t currentShardIndex{};
		std::atomic_uint32_t totalShardCount{};
		std::atomic_int32_t workerCount{ -1 };
		std::atomic_int32_t guildQuantity{};
		ShardingOptions shardingOptions{};
		ObjectGenerator randomizer{};
		ConfigParser configParser{};
#ifdef _WIN32
		WSADataWrapper theWSAData{};
#endif
		std::mutex coutMutex{};
		std::mutex theMutex{};


		void connectShard();
	};
	/**@}*/
}// namespace DiscordCoreLoader
