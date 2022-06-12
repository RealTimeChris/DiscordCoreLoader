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
/// https://discordcoreapi.com
/// \file SSLClients.hpp

#pragma once

#ifndef OPENSSL_NO_DEPRECATED
	#define OPENSSL_NO_DEPRECATED
#endif

#ifndef FD_SETSIZE
	#define FD_SETSIZE 400
#endif

#ifdef _WIN32
	#pragma comment(lib, "Ws2_32.lib")
	#include <WS2tcpip.h>
	#include <WinSock2.h>
#else
	#include <fcntl.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <stdint.h>
	#include <sys/epoll.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <arpa/inet.h>
#endif

#include <algorithm>
#include <filesystem>
#include <discordcoreloader/FoundationEntities.hpp>
#include <openssl/err.h>
#include <openssl/ssl.h>

#ifdef max
	#undef max
#endif

#ifdef min
	#undef min
#endif

namespace DiscordCoreLoader {

	using SOCKET = int32_t;

	class BaseSocketAgent;

	enum class WebSocketOpCode : int8_t { Op_Continuation = 0x00, Op_Text = 0x01, Op_Binary = 0x02, Op_Close = 0x08, Op_Ping = 0x09, Op_Pong = 0x0a };

	struct MessagePackage {
		std::vector<std::string> theStrings{};
	};

	struct WebSocketMessage {
		WebSocketMessage& operator=(WebSocketMessage&& other) noexcept {
			this->stringMsg = std::move(other.stringMsg);
			this->jsonMsg = std::move(other.jsonMsg);
			this->theOpCode = other.theOpCode;
			return *this;
		}
		WebSocketMessage(WebSocketMessage&& other) noexcept {
			*this = std::move(other);
		}
		WebSocketMessage& operator=(const WebSocketMessage& other) noexcept {
			this->theOpCode = other.theOpCode;
			this->stringMsg = other.stringMsg;
			this->jsonMsg = other.jsonMsg;
			return *this;
		}
		WebSocketMessage(const WebSocketMessage& other) noexcept {
			*this = other;
		}
		WebSocketMessage() = default;
		WebSocketOpCode theOpCode{};
		nlohmann::json jsonMsg{};
		std::string stringMsg{};
	};

#ifndef SOCKET_ERROR
	#define SOCKET_ERROR (-1)
#endif

	void reportError(const std::string& errorPosition, int32_t errorValue) noexcept;
#ifdef _WIN32
	struct WSADataWrapper {
		struct WSADataDeleter {
			void operator()(WSADATA* other) {
				WSACleanup();
				delete other;
			}
		};

		WSADataWrapper() {
			if (auto errorValue = WSAStartup(MAKEWORD(2, 2), this->thePtr.get()); errorValue != 0) {
				std::cout << shiftToBrightRed() << "WSAStartup() Error: " << errorValue << ", ";
				std::cout << std::endl << reset();
			}
		}

	  protected:
		std::unique_ptr<WSADATA, WSADataDeleter> thePtr{ new WSADATA{}, WSADataDeleter{} };
	};
#endif

	struct addrinfoWrapper {
		addrinfo* operator->() {
			if (this->addrinfoPtrTwo == nullptr) {
				throw std::runtime_error{ "Error: addrinfoPtrTwo was nullptr." };
			}
			return this->addrinfoPtrTwo;
		}

		operator addrinfo**() {
			this->doWeClearAddrInfo = true;
			if (this->addrinfoPtrTwo == nullptr) {
				throw std::runtime_error{ "Error: addrinfoPtrTwo was nullptr." };
			}
			return &this->addrinfoPtrTwo;
		}

		operator addrinfo*() {
			if (this->addrinfoPtrTwo == nullptr) {
				throw std::runtime_error{ "Error: addrinfoPtrTwo was nullptr." };
			}
			return this->addrinfoPtrTwo;
		}

		addrinfoWrapper(nullptr_t){};

		~addrinfoWrapper() {
			if (this->doWeClearAddrInfo) {
				freeaddrinfo(this->addrinfoPtrTwo);
			} else {
				delete this->addrinfoPtrTwo;
			}
		}

	  protected:
		addrinfo* addrinfoPtrTwo{ new addrinfo{} };
		bool doWeClearAddrInfo{ false };
	};

	struct SSL_CTXWrapper {
		struct SSL_CTXDeleter {
			void operator()(SSL_CTX* other) {
				if (other) {
					SSL_CTX_free(other);
					other = nullptr;
				}
			}
		};

		SSL_CTXWrapper& operator=(SSL_CTX* other) {
			this->sslCTXPtr.reset(other);
			if (!SSL_CTX_up_ref(other)) {
				std::cout << shiftToBrightRed() << "SSL_CTX_up_ref() Error: ";
				ERR_print_errors_fp(stdout);
				std::cout << std::endl << reset();
			}
			return *this;
		}

		operator SSL_CTX*() {
			return this->sslCTXPtr.get();
		}

		SSL_CTXWrapper(nullptr_t){};

	  protected:
		std::unique_ptr<SSL_CTX, SSL_CTXDeleter> sslCTXPtr{ nullptr, SSL_CTXDeleter{} };
	};

	struct SSLWrapper {
		struct SSLDeleter {
			void operator()(SSL* other) {
				if (other) {
					SSL_shutdown(other);
					SSL_free(other);
					other = nullptr;
				}
			}
		};

		SSLWrapper& operator=(SSL* other) {
			this->sslPtr.reset(other);
			if (!SSL_up_ref(other)) {
				std::cout << shiftToBrightRed() << "SSL_up_ref() Error: ";
				ERR_print_errors_fp(stdout);
				std::cout << std::endl << reset();
			}
			return *this;
		}

		operator SSL*() {
			return this->sslPtr.get();
		}

		SSLWrapper(nullptr_t){};

	  protected:
		std::unique_ptr<SSL, SSLDeleter> sslPtr{ nullptr, SSLDeleter{} };
	};

	struct SOCKETWrapper {
		struct SOCKETDeleter {
			void operator()(SOCKET* other) {
				if (other) {
#ifdef _WIN32
					shutdown(*other, SD_BOTH);
					closesocket(*other);
#else
					shutdown(*other, SHUT_RDWR);
					close(*other);
#endif
					*other = -1;
				}
			}
		};

		SOCKETWrapper& operator=(SOCKETWrapper&& other) noexcept {
			this->socketPtr.swap(other.socketPtr);
			*other.socketPtr = SOCKET_ERROR;
			other.socketPtr = nullptr;
			return *this;
		}

		SOCKETWrapper(SOCKETWrapper&& other) noexcept {
			*this = std::move(other);
		}

		SOCKETWrapper& operator=(SOCKET other) {
			*this->socketPtr = other;
			return *this;
		}

		SOCKETWrapper(SOCKET other) {
			*this = other;
		}

		operator SOCKET() {
			return *this->socketPtr;
		}

		SOCKETWrapper(nullptr_t) {
		}

	  protected:
		std::unique_ptr<SOCKET, SOCKETDeleter> socketPtr{ new SOCKET{ static_cast<SOCKET>(SOCKET_ERROR) }, SOCKETDeleter{} };
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

	class WebSocketSSLShard {
	  public:
		friend class WebSocketSSLServerMain;
		friend class BaseSocketAgent;

		WebSocketSSLShard& operator=(WebSocketSSLShard&& other) noexcept;

		WebSocketSSLShard(WebSocketSSLShard&& other) noexcept;

		WebSocketSSLShard() = default;

		WebSocketSSLShard(SOCKET theSocket, SSL_CTX* theContext, bool doWePrintErrorsNew);

		void writeData(std::string& data) noexcept;

		std::string& getInputBuffer() noexcept;

		bool checkIfConnected() noexcept;

		uint64_t getBytesRead() noexcept;

		~WebSocketSSLShard();

	  protected:
		std::queue<WebSocketMessage> theMessageQueue{};
		uint64_t maxBufferSize{ (1024 * 16) - 1 };
		std::vector<std::string> outputBuffer{};
		SOCKETWrapper clientSocket{ nullptr };
		MessagePackage theCurrentMessage{};
		int32_t currentReconnectTries{ 0 };
		SSL_CTX* theContext{ nullptr };
		uint64_t currentReadSize{ 0 };
		bool areWeConnected{ false };
		bool doWePrintError{ false };
		int64_t currentGuildCount{};
		uint64_t bytesWritten{ 0 };
		int64_t totalGuildCount{};
		SSLWrapper ssl{ nullptr };
		std::string inputBuffer{};
		int64_t lastNumberSent{};
		bool sendGuilds{ false };
		uint64_t bytesRead{ 0 };
		nlohmann::json shard{};
		bool wantWrite{ true };
		bool wantRead{ true };
	};

	struct ReconnectionPackage {
		std::unordered_map<SOCKET, std::unique_ptr<WebSocketSSLShard>>* theMap{};
		int32_t totalShardCount{};
		int32_t currentShard{};
		SOCKET theSocket{};
	};

	class WebSocketSSLServerMain {
	  public:
		friend class WebSocketSSLShard;

		WebSocketSSLServerMain() = default;

		WebSocketSSLServerMain(const std::string& theUrl, const std::string& port, bool doWePrintError, std::atomic_bool* doWeQuit);

		ProcessIOReturnData processIO(std::unordered_map<SOCKET, std::unique_ptr<WebSocketSSLShard>>& theMap) noexcept;

		std::unique_ptr<WebSocketSSLShard> connectShard(SOCKET newShard);

		void submitReconnectionShard(ReconnectionPackage);

		ReconnectionPackage reconnectShard();

		SOCKET getNewSocket();

	  protected:
		std::queue<ReconnectionPackage> theReconnections{};
		SOCKETWrapper theServerSocket{ nullptr };
		addrinfoWrapper addrInfo{ nullptr };
		int64_t timeOfLastConnection{ 0 };
		SSL_CTXWrapper context{ nullptr };
		bool doWePrintError{ false };
		std::atomic_bool* doWeQuit{};
		std::mutex theMutex{};
		std::string baseUrl{};
		std::string port{};
	};

}// namespace DiscordCoreLoader