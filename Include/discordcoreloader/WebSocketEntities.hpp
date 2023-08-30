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
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file WebSocketEntities.hpp

#pragma once

#include <discordcoreloader/ObjectGenerator.hpp>
#include <discordcoreloader/SSLClients.hpp>
#include <discordcoreloader/ErlParser.hpp>
#include <Jsonifier/Index.hpp>

namespace DiscordCoreLoader {

	struct ConnectionPackage {
		bool isThisOurFirstConnection{ true };
		int32_t currentReconnectTries{ 0 };
		std::atomic_bool areWeConnected{};
		int32_t currentShardIndex{ 0 };
	};

	class WebSocketSSLShard : public SSLClient {
	  public:
		friend class WebSocketSSLServerMain;
		friend class DiscordCoreClient;
		friend class BaseSocketAgent;

		WebSocketSSLShard(SOCKET theSocket, SSL_CTX* theContext, bool doWePrintErrorsNew, BaseSocketAgent* theAgent);

		void handleBuffer() noexcept;

	  protected:
		BaseSocketAgent* theAgent{ nullptr };
	};

	enum class WebSocketCloseCode : uint16_t {
		Unknown_Error		  = 4000,///< We're not sure what went wrong. Try reconnecting?
		Unknown_Opcode		  = 4001,///< You sent an invalid Gateway opcode or an invalid payload for an opcode. Don't do that!
		Decode_Error		  = 4002,///< You sent an invalid payload to us. Don't do that!
		Not_Authenticated	  = 4003,///< You sent us a payload prior to identifying.
		Authentication_Failed = 4004,///< The account token sent with your identify payload is incorrect.
		Already_Authenticated = 4005,///< You sent more than one identify payload. Don't do that!
		Invalid_Seq			  = 4007,///<	The sequence sent when resuming the session was invalid. Reconnect and start a new session.
		Rate_Limited		  = 4008,///< Woah nelly! You're sending payloads to us too quickly. Slow it down! You will be disconnected on receiving this.
		Session_Timed_Out	  = 4009,///< Your session timed out. Reconnect and start a new one.
		Invalid_Shard		  = 4010,///< You sent us an invalid shard when identifying.
		Sharding_Required	  = 4011,///< The session would have handled too many guilds - you are required to shard your connection in order to connect.
		Invalid_API_Version	  = 4012,///< You sent an invalid version for the gateway.
		Invalid_Intent		  = 4013,///< You sent an invalid intent for a Gateway Intent. You may have incorrectly calculated the bitwise value.
		Disallowed_Intent	  = 4014///< You sent a disallowed intent for a Gateway Intent. You may have tried to specify an intent that you have not enabled.
	};

	class BaseSocketAgent : public EtfParser {
	  public:
		friend class DiscordCoreClient;
		friend class WebSocketSSLShard;

		BaseSocketAgent(WebSocketSSLServerMain* webSocketSSLServerMainNew, DiscordCoreClient* discordCoreClient, std::atomic_bool* doWeQuitNew,
			bool doWeInstantiateAThread) noexcept;

		void sendMessage(Jsonifier::String&& dataToSend, WebSocketOpCode theOpCode, SSLClient* theShard, bool priority) noexcept;

		void sendMessage(Jsonifier::String* dataToSend, SSLClient* theShard, bool priority) noexcept;

		std::jthread* getTheTask() noexcept;

		~BaseSocketAgent() noexcept;

	  protected:
		std::unordered_map<SOCKET, std::unique_ptr<WebSocketSSLShard>> theClients{};
		std::unordered_map<SOCKET, Jsonifier::Vector<UnavailableGuild>> theGuilds{};
		GatewayIntents intentsValue{ GatewayIntents::All_Intents };
		WebSocketSSLServerMain* webSocketSSLServerMain{ nullptr };
		std::unique_ptr<std::jthread> theTask{ nullptr };
		DiscordCoreClient* discordCoreClient{ nullptr };
		std::atomic_bool* doWeQuit{ nullptr };
		std::atomic_int32_t workerCount{ -1 };
		int32_t heartbeatInterval{ 45000 };
		Jsonifier::JsonifierCore parser{};
		ObjectGenerator randomizer{};
		int32_t currentClientSize{};
		Jsonifier::String stringBuffer{};
		uint16_t closeCode{ 0 };
		Jsonifier::String sessionId{};

		Jsonifier::Vector<Jsonifier::String> tokenize(const Jsonifier::String& dataIn, SSLClient* theShard, const Jsonifier::String& separator = "\r\n") noexcept;

		void createHeader(Jsonifier::String& outBuffer, uint64_t sendLength, WebSocketOpCode opCodeNew) noexcept;

		void onMessageReceived(WebSocketSSLShard* theShard, Jsonifier::String& theString) noexcept;

		void initDisconnect(WebSocketCloseCode reason, SSLClient* theShard) noexcept;

		void sendGuildMemberChunks(SSLClient* theShard) noexcept;

		void sendResumedPayload(SSLClient* theShard) noexcept;

		void sendCreateGuilds(SSLClient* theShard) noexcept;

		void sendReadyMessage(SSLClient* theShard) noexcept;

		void sendFinalMessage(SSLClient* theShard) noexcept;

		void sendHelloMessage(SSLClient* theShard) noexcept;

		void sendHeartBeat(SSLClient* theShard) noexcept;

		void handleBuffer(SSLClient* theShard) noexcept;

		bool parseHeader(SSLClient* theShard) noexcept;

		void onClosed(SSLClient* theShard) noexcept;

		void run(std::stop_token theToken) noexcept;

		void generateGuildData() noexcept;
	};
}// namespace DiscordCoreLoader
