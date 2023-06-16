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
/// SSLClents.hpp - Header file for the "SSL Client" stuff.
/// May 22, 2022
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file SSLClients.hpp

#pragma once

#ifndef OPENSSL_NO_DEPRECATED
	#define OPENSSL_NO_DEPRECATED
#endif

#ifndef FD_SETSIZE
	#define FD_SETSIZE 400
#endif

#include <algorithm>
#include <filesystem>
#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/ConfigParser.hpp>

extern "C" {
	#undef APPMACROS_ONLY
	#include <openssl/err.h>
	#include <openssl/ssl.h>
}

#ifdef max
	#undef max
#endif

#ifdef min
	#undef min
#endif

#ifdef _WIN32
	#pragma comment(lib, "Ws2_32.lib")
	#include <WinSock2.h>
	#include <WS2tcpip.h>
	#define poll(fd_set, fd_count, timeout) WSAPoll(fd_set, fd_count, timeout)
#elif __linux__
	#include <fcntl.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <stdint.h>
	#include <sys/epoll.h>
	#include <poll.h>
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <unistd.h>
#endif

namespace DiscordCoreLoader {

#ifndef SOCKET_ERROR
	#define SOCKET_ERROR (-1)
#endif

#ifdef _WIN32
	using SOCKET = uint32_t;
#else
	using SOCKET = int32_t;
#endif

	struct ConnectionError : public std::runtime_error {
		int32_t shardNumber{};
		explicit ConnectionError(const std::string& theString);
	};

	enum class WebSocketMode : int8_t { JSON = 0, ETF = 1 };

	struct PollFDWrapper {
		std::vector<uint32_t> theIndices{};
		std::vector<pollfd> thePolls{};
	};

	class BaseSocketAgent;

	struct MessagePackage {
		std::vector<std::string> theStrings{};
	};

	struct WebSocketMessage {
		WebSocketMessage& operator=(WebSocketMessage&& other) noexcept {
			this->theOpCode = other.theOpCode;
			this->t = std::move(other.t);
			this->d = std::move(other.d);
			this->op = other.op;
			this->s = other.s;
			return *this;
		}
		WebSocketMessage(WebSocketMessage&& other) noexcept {
			*this = std::move(other);
		}
		WebSocketMessage& operator=(const WebSocketMessage& other) noexcept {
			this->theOpCode = other.theOpCode;
			this->op = other.op;
			this->t = other.t;
			this->d = other.d;
			this->s = other.s;
			return *this;
		}
		WebSocketMessage(const WebSocketMessage& other) noexcept {
			*this = other;
		}
		WebSocketMessage() = default;
		WebSocketOpCode theOpCode{};
		Jsonifier::RawJsonData d{};
		std::string t{};
		int32_t op{};
		int32_t s{};
	};

	std::string reportError(const char* errorPosition, int32_t errorValue) noexcept;

#ifdef _WIN32
	struct WSADataWrapper {
		struct WSADataDeleter {
			void operator()(WSADATA* other);
		};

		WSADataWrapper();

	  protected:
		std::unique_ptr<WSADATA, WSADataDeleter> ptr{ new WSADATA{}, WSADataDeleter{} };
	};
#endif

	struct SSL_CTXWrapper {
		struct SSL_CTXDeleter {
			void operator()(SSL_CTX* other);
		};

		SSL_CTXWrapper& operator=(SSL_CTX* other);

		operator SSL_CTX*();

		SSL_CTXWrapper() noexcept = default;

	  protected:
		std::unique_ptr<SSL_CTX, SSL_CTXDeleter> ptr{ nullptr, SSL_CTXDeleter{} };
	};

	struct SSLWrapper {
		struct SSLDeleter {
			void operator()(SSL* other);
		};

		SSLWrapper& operator=(nullptr_t other);

		SSLWrapper& operator=(SSL* other);

		operator SSL*();

		SSLWrapper() noexcept = default;

	  protected:
		std::unique_ptr<SSL, SSLDeleter> ptr{ nullptr, SSLDeleter{} };
	};

	struct SOCKETWrapper {
		struct SOCKETDeleter {
			void operator()(SOCKET* other);
		};

		SOCKETWrapper& operator=(SOCKETWrapper&&) noexcept;

		SOCKETWrapper(SOCKETWrapper&&) noexcept;

		SOCKETWrapper& operator=(SOCKET other) noexcept;

		SOCKETWrapper(SOCKET other) noexcept;

		operator SOCKET*() noexcept;

		operator SOCKET() noexcept;

		SOCKETWrapper() noexcept = default;

	  protected:
		std::unique_ptr<SOCKET, SOCKETDeleter> ptr{ new SOCKET{ static_cast<SOCKET>(SOCKET_ERROR) }, SOCKETDeleter{} };
	};

	struct sockaddrWrapper {
		sockaddr* operator->();

		operator sockaddr_in*();

		operator sockaddr*();

		sockaddrWrapper() noexcept = default;

		~sockaddrWrapper() noexcept = default;

	  protected:
		sockaddr_in ptr{};
	};

	struct addrinfoWrapper {
		addrinfo* operator->();

		operator addrinfo**();

		operator addrinfo*();

		addrinfoWrapper() noexcept = default;

		~addrinfoWrapper();

	  protected:
		addrinfo* ptr{ new addrinfo{} };
		bool doWeClearAddrInfo{ false };
	};

	enum class ProcessIOReturnCode : int32_t {
		Error = -1,///< Error occurred.
		Success = 0,///< Succesful read or write occurred.
	};

	struct ProcessIOReturnData {
		ProcessIOReturnCode returnCode{};
		int32_t writtenOrReadCount{ 0 };
		SOCKET returnIndex{};
	};

	enum class WebSocketState : int8_t { Initializing = 0, Connected = 1 };

	class BaseSocketAgent;

	class SSLClient {
	  public:
		friend class WebSocketSSLServerMain;
		friend class DiscordCoreClient;
		friend class BaseSocketAgent;

		SSLClient& operator=(SSLClient&& other) noexcept;

		SSLClient(SSLClient&& other) noexcept;

		SSLClient() noexcept = default;

		SSLClient(SOCKET theSocket, SSL_CTX* theContextNew, bool doWePrintErrorsNew);

		void writeData(std::string& data, bool priority) noexcept;

		virtual void handleBuffer() noexcept = 0;

		std::string& getInputBuffer() noexcept;

		bool areWeStillConnected() noexcept;

		uint64_t getBytesRead() noexcept;

		bool writeDataProcess() noexcept;

		bool readDataProcess() noexcept;

		void disconnect() noexcept;

		~SSLClient() noexcept;

	  protected:
		StopWatch<std::chrono::milliseconds> theStopWatch{ std::chrono::milliseconds{ 1 } };
		WebSocketOpCode theOpCode{ WebSocketOpCode::Op_Binary };
		std::deque<WebSocketMessage> theMessageQueue{};
		const uint64_t maxBufferSize{ (1024 * 16) - 1 };
		std::array<char, 1024 * 16> rawInputBuffer{};
		SOCKETWrapper clientSocket{};
		std::deque<std::string> outputBuffers{};
		MessagePackage theCurrentMessage{};
		int32_t currentReconnectTries{ 0 };
		uint32_t currentSocketIndex{ 0 };
		bool doWeHaveOurGuild{ false };
		SSL_CTX* theContext{ nullptr };
		std::string serverToClientBuffer{};
		bool areWeConnected{ false };
		bool doWePrintError{ false };
		GuildData theGuildHolder{};
		int64_t currentGuildCount{};
		WebSocketState theState{};
		bool sendGuilds{ false };
		uint64_t bytesWritten{ 0 };
		WebSocketMode theMode{};
		int64_t totalGuildCount{};
		int64_t lastNumberSent{};
		uint64_t bytesRead{ 0 };
		std::string inputBuffer{};
		uint32_t shard[2]{};
		std::string authKey{};
		SSLWrapper ssl{};
	};

	struct ReconnectionPackage {
		std::unordered_map<SOCKET, std::unique_ptr<SSLClient>>* theMap{};
		int32_t currentShardIndex{};
		int32_t totalShardCount{};
	};
	class WebSocketSSLShard;
	class WebSocketSSLServerMain {
	  public:
		friend class DiscordCoreClient;
		friend class SSLClient;

		WebSocketSSLServerMain() = default;

		WebSocketSSLServerMain(const std::string& theUrl, const std::string& port, bool doWePrintError, std::atomic_bool* doWeQuit,
			ConfigParser* theData);

		std::vector<WebSocketSSLShard*> processIO(std::vector<WebSocketSSLShard*>& theMap) noexcept;

		SOCKET getNewSocket();

	  protected:
		std::queue<ReconnectionPackage> theReconnections{};
		const int32_t maxBufferSize{ 1024 * 16 };
		SOCKETWrapper theServerSocket{};
		ConfigParser* theConfigParser{ nullptr };
		addrinfoWrapper addrInfo{};
		SSL_CTXWrapper context{};
		bool doWePrintError{ false };
		std::atomic_bool* doWeQuit{};
		std::mutex theMutex{};
		std::string baseUrl{};
		std::string port{};
	};

}// 