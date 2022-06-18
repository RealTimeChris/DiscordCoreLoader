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
/// WebSocketEntities.hpp - Header for the webSocket related classes and
/// May 22, 2022
/// https://discordcoreapi.com
/// \file WebSocketEntities.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/ErlPacker.hpp>
#include <discordcoreloader/SSLClients.hpp>
#include <discordcoreloader/JSONIfier.hpp>
#include <discordcoreloader/ObjectGenerator.hpp>

namespace DiscordCoreLoader {

	enum class WebSocketMode : int8_t { JSON = 0, ETF = 1 };

	enum class WebSocketCloseCode : uint16_t {
		Unknown_Error = 4000,///< We're not sure what went wrong. Try reconnecting?
		Unknown_Opcode = 4001,///< You sent an invalid Gateway opcode or an invalid payload for an opcode. Don't do that!
		Decode_Error = 4002,///< You sent an invalid payload to us. Don't do that!
		Not_Authenticated = 4003,///< You sent us a payload prior to identifying.
		Authentication_Failed = 4004,///< The account token sent with your identify payload is incorrect.
		Already_Authenticated = 4005,///< You sent more than one identify payload. Don't do that!
		Invalid_Seq = 4007,///<	The sequence sent when resuming the session was invalid. Reconnect and start a new session.
		Rate_Limited = 4008,///< Woah nelly! You're sending payloads to us too quickly. Slow it down! You will be disconnected on receiving this.
		Session_Timed_Out = 4009,///< Your session timed out. Reconnect and start a new one.
		Invalid_Shard = 4010,///< You sent us an invalid shard when identifying.
		Sharding_Required = 4011,///< The session would have handled too many guilds - you are required to shard your connection in order to connect.
		Invalid_API_Version = 4012,///< You sent an invalid version for the gateway.
		Invalid_Intent = 4013,///< You sent an invalid intent for a Gateway Intent. You may have incorrectly calculated the bitwise value.
		Disallowed_Intent = 4014///< You sent a disallowed intent for a Gateway Intent. You may have tried to specify an intent that you have not enabled.
	};

	class BaseSocketAgent {
	  public:
		friend DiscordCoreClient;

		BaseSocketAgent(WebSocketSSLServerMain* webSocketSSLServerMainNew, DiscordCoreClient* discordCoreClient, std::atomic_bool* doWeQuitNew) noexcept;

		void sendMessage(std::string* dataToSend, WebSocketSSLShard& theIndex) noexcept;

		void sendMessage(nlohmann::json& dataToSend, WebSocketSSLShard& theIndex) noexcept;

		std::jthread* getTheTask() noexcept;

		~BaseSocketAgent() noexcept;

	  protected:
		std::unordered_map<SOCKET, std::unique_ptr<WebSocketSSLShard>> theClients{};
		std::unordered_map<SOCKET, std::vector<UnavailableGuild>> theGuilds{};
		GatewayIntents intentsValue{ GatewayIntents::All_Intents };
		WebSocketSSLServerMain* webSocketSSLServerMain{ nullptr };
		WebSocketOpCode opCode{ WebSocketOpCode::Op_Binary };
		std::unordered_map<SOCKET, std::string> authKeys{};
		std::unique_ptr<std::jthread> theTask{ nullptr };
		DiscordCoreClient* discordCoreClient{ nullptr };
		SOCKETWrapper currentNewSocket{ nullptr };
		std::atomic_bool* doWeQuit{ nullptr };
		std::atomic_bool doWeConnect{ false };
		int32_t heartbeatInterval{ 41500 };
		SOCKET currentIndex{ 0 };
		WebSocketMode theMode{};
		std::string sessionId{};
		uint16_t closeCode{ 0 };
		ErlPacker erlPacker{};
		JSONIFier jsonifier{};

		std::vector<std::string> tokenize(const std::string& dataIn, WebSocketSSLShard& theIndex, const std::string& separator = "\r\n") noexcept;

		uint64_t createHeader(std::string& outBuffer, uint64_t sendLength, WebSocketOpCode opCodeNew) noexcept;

		void stringifyJsonData(const nlohmann::json& jsonData, std::string& theString) noexcept;

		void initDisconnect(WebSocketCloseCode reason, WebSocketSSLShard& theIndex) noexcept;

		void respondToDisconnect(WebSocketSSLShard& theIndex) noexcept;

		void onMessageReceived(WebSocketSSLShard& theIndex) noexcept;

		void sendReadyMessage(WebSocketSSLShard& theIndex) noexcept;

		void sendFinalMessage(WebSocketSSLShard& theIndex) noexcept;

		void sendHelloMessage(WebSocketSSLShard& theIndex) noexcept;

		void sendCreateGuild(WebSocketSSLShard& theIndex) noexcept;

		void sendHeartBeat(WebSocketSSLShard& theIndex) noexcept;

		bool parseHeader(WebSocketSSLShard& theIndex) noexcept;

		void run(std::stop_token theToken) noexcept;

		void sendGuildMemberChunks() noexcept;

		void sendFinalMessage() noexcept;

		void connectInternal() noexcept;

		void connect() noexcept;
	};

	enum class GeneratorAgentWorkloadTypes {
		Guild_Create = 0,
	};

	class GeneratorAgent {
	  public:

		GeneratorAgent& operator=(GeneratorAgent&& other) {
			this->theLastNumbersSent = other.theLastNumbersSent;
			this->theWorkloadBuffer = other.theWorkloadBuffer;
			this->theSendBuffer = other.theSendBuffer;
			this->theTask.swap(other.theTask);
			this->erlPacker = other.erlPacker;
			this->jsonifier = other.jsonifier;
			this->doWeQuit = other.doWeQuit;
			this->theMode = other.theMode;
			return *this;
		}

		GeneratorAgent(GeneratorAgent&& other) {
			*this = std::move(other);
		}

		GeneratorAgent() = default;

		GeneratorAgent(std::atomic_bool* doWeQuit, JSONIFier* jsonifier, WebSocketMode theMode, ErlPacker* erlPackerNew);

		void placeOrder(int32_t, int32_t, GeneratorAgentWorkloadTypes) noexcept;

		std::string collectWorkload(int32_t) noexcept;

	  protected:
		std::unordered_map<int32_t, std::queue<GeneratorAgentWorkloadTypes>> theWorkloadBuffer{};
		std::unordered_map<int32_t, std::queue<std::string>> theSendBuffer{};
		std::unordered_map<int32_t, int32_t> theLastNumbersSent{};
		std::atomic_bool* doWeQuit{ nullptr };
		JSONIFier* jsonifier{ nullptr };
		ErlPacker* erlPacker{ nullptr };
		nlohmann::json guildHolder{};
		std::mutex theAccessMutex{};
		WebSocketMode theMode{};
		std::jthread theTask{};

		void createHeader(std::string& outBuffer, uint64_t sendLength, WebSocketOpCode opCodeNew) noexcept;

		void stringifyJsonData(const nlohmann::json& jsonData, std::string& theString) noexcept;

		void generateGuildCreate(int32_t theIndex);
				
		void run(std::stop_token theToken);
	};

}// namespace DiscordCoreLoader
