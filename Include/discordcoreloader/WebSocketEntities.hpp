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
/// WebSocketEntities.hpp - Header for the webSocket related classes and
/// May 22, 2022
/// https://github.com/RealTimeChris/discord_core_loader
/// \file WebSocketEntities.hpp

#pragma once

#include <discordcoreloader/SSLClients.hpp>
#include <discordcoreloader/JSONIfier.hpp>
#include <discordcoreloader/JsonSpecializations.hpp>
#include <discordcoreloader/Etf.hpp>

namespace discord_core_loader {

	struct ConnectionPackage {
		std::unique_ptr<std::atomic_bool> areWeConnected{ std::make_unique<std::atomic_bool>() };
		bool isThisOurFirstConnection{ true };
		int32_t currentReconnectTries{ 0 };
		int32_t currentShardIndex{ 0 };
	};

	class WebSocketSSLShard : public SSLClient {
	  public:
		friend class WebSocketSSLServerMain;
		friend class DiscordCoreClient;
		friend class BaseSocketAgent;

		WebSocketSSLShard(bool doWePrintErrorsNew, BaseSocketAgent* theAgent);

		void connect(SSL_CTX* contextNew);

		void handleBuffer() noexcept;

	  protected:
		BaseSocketAgent* theAgent{ nullptr };
	};

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
		Disallowed_Intent =
			4014///< You sent a disallowed intent for a Gateway Intent. You may have tried to specify an intent that you have not enabled.
	};

	class BaseSocketAgent : public etf_parser {
	  public:
		friend class DiscordCoreClient;
		friend class WebSocketSSLShard;

		BaseSocketAgent(DiscordCoreClient* discordCoreClient, std::atomic_bool* doWeQuitNew, bool doWeInstantiateAThread) noexcept;

		void sendMessage(etf_serializer&& dataToSend, WebSocketOpCode theOpCode, WebSocketSSLShard* theShard, bool priority) noexcept;

		void sendMessage(jsonifier::string* dataToSend, WebSocketSSLShard* theShard, bool priority) noexcept;

		std::jthread* getTheTask() noexcept;

		~BaseSocketAgent() noexcept;

	  protected:
		std::unordered_map<SOCKET, std::unique_ptr<WebSocketSSLShard>> theClients{};
		std::unordered_map<SOCKET, jsonifier::vector<UnavailableGuild>> theGuilds{};
		std::unique_ptr<WebSocketSSLServerMain> webSocketSSLServerMain{};
		GatewayIntents intentsValue{ GatewayIntents::All_Intents };
		std::unique_ptr<std::jthread> theTask{ nullptr };
		DiscordCoreClient* discordCoreClient{ nullptr };
		std::atomic_bool* doWeQuit{ nullptr };
		std::atomic_int32_t workerCount{ -1 };
		int32_t heartbeatInterval{ 45000 };
		jsonifier::string sessionId{};
		int32_t currentClientSize{};
		uint16_t closeCode{ 0 };
		JSONIFier jsonifier{};


		jsonifier::vector<jsonifier::string> tokenize(const jsonifier::string& dataIn, WebSocketSSLShard* theShard, const jsonifier::string& separator = "\r\n") noexcept;

		void createHeader(jsonifier::string& outBuffer, uint64_t sendLength, WebSocketOpCode opCodeNew) noexcept;

		void onMessageReceived(WebSocketSSLShard* theShard, jsonifier::string& theString) noexcept;

		void initDisconnect(WebSocketCloseCode reason, WebSocketSSLShard* theShard) noexcept;
		
		void connectShard(std::unique_ptr<WebSocketSSLShard>&) noexcept;

		void sendGuildMemberChunks(WebSocketSSLShard* theShard) noexcept;

		void sendResumedPayload(WebSocketSSLShard* theShard) noexcept;

		void sendCreateGuilds(WebSocketSSLShard* theShard) noexcept;

		void sendReadyMessage(WebSocketSSLShard* theShard) noexcept;

		void sendFinalMessage(WebSocketSSLShard* theShard) noexcept;

		void sendHelloMessage(WebSocketSSLShard* theShard) noexcept;

		void sendHeartBeat(WebSocketSSLShard* theShard) noexcept;

		void handleBuffer(WebSocketSSLShard* theShard) noexcept;

		bool parseHeader(WebSocketSSLShard* theShard) noexcept;

		void onClosed(WebSocketSSLShard* theShard) noexcept;

		void run(std::stop_token theToken) noexcept;

		void generateGuildData() noexcept;
	};
}// namespace discord_core_loader
