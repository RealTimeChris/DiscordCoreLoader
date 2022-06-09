/*
*
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
/// SSLClents.cpp - Source file for the "SSL Client" stuff.
/// May 22, 2022
/// https://discordcoreapi.com
/// \file SSLClients.cpp

#include <discordcoreloader/SSLClients.hpp>
#include <discordcoreloader/WebSocketEntities.hpp>

namespace DiscordCoreLoader {

	std::string getCurrentPath() {
		std::stringstream theStream{};
		theStream << std::filesystem::current_path();
		return theStream.str().substr(1, theStream.str().size() - 2);
	}

	void reportSSLError(const std::string& errorPosition, int32_t errorValue = 0, SSL* ssl = nullptr) noexcept {
		if (ssl) {
			std::cout << shiftToBrightRed() << errorPosition << SSL_get_error(ssl, errorValue) << std::endl;
		} else {
			std::cout << shiftToBrightRed() << errorPosition << std::endl;
		}
		ERR_print_errors_fp(stdout);
		std::cout << std::endl << reset();
	}

	void reportError(const std::string& errorPosition, int32_t errorValue) noexcept {
		std::cout << shiftToBrightRed() << errorPosition << errorValue << ", ";
#ifdef _WIN32
		std::unique_ptr<char[]> string{ std::make_unique<char[]>(1024) };
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), string.get(), 1024, NULL);
		std::cout << WSAGetLastError() << ", " << string << std::endl << reset();
#else
		std::cout << strerror(errno) << std::endl << reset();
#endif
	}

	WebSocketSSLShard::WebSocketSSLShard(SOCKET theSocket, SSL_CTX* theContextNew, bool doWePrintErrorsNew, int32_t currentShard,
		int32_t totalNumberOfShards)
		: maxBufferSize(1024 * 16) {
		this->doWePrintError = doWePrintErrorsNew;
		this->theContext = theContextNew;
		this->shard = nlohmann::json::array();
		this->shard.push_back(currentShard);
		this->shard.push_back(totalNumberOfShards);
		this->clientSocket = theSocket;

		if (this->clientSocket != SOCKET_ERROR) {
			if (this->ssl = SSL_new(this->theContext); this->ssl == nullptr) {
				if (this->doWePrintError) {
					reportSSLError("SSL_new() Error: ");
				}
				return;
			}

			if (auto returnValue = SSL_set_fd(this->ssl, this->clientSocket); !returnValue) {
				if (this->doWePrintError) {
					reportSSLError("SSL_set_fd() Error: ", returnValue, this->ssl);
				}
				return;
			}

			while (SSL_accept(this->ssl) != 1) {
			}
		}
	};

	WebSocketSSLShard& WebSocketSSLShard::operator=(WebSocketSSLShard&& other) noexcept {
		if (this != &other) {
			this->outputBuffer = std::move(other.outputBuffer);
			this->clientSocket = std::move(other.clientSocket);
			this->inputBuffer = std::move(other.inputBuffer);
			this->doWePrintError = other.doWePrintError;
			this->maxBufferSize = other.maxBufferSize;
			this->bytesWritten = other.bytesWritten;
			this->bytesRead = other.bytesRead;
			this->ssl = std::move(other.ssl);
			this->wantRead = other.wantRead;
			this->theContext = theContext;
		}
		return *this;
	}

	WebSocketSSLShard::WebSocketSSLShard(WebSocketSSLShard&& other) noexcept {
		*this = std::move(other);
	}

	void WebSocketSSLShard::writeData(std::string& data) noexcept {
		this->outputBuffer.push_back(std::move(data));
	}

	std::string& WebSocketSSLShard::getInputBuffer() noexcept {
		return this->inputBuffer;
	}

	uint64_t WebSocketSSLShard::getBytesRead() noexcept {
		return this->bytesRead;
	}

	WebSocketSSLServerMain::WebSocketSSLServerMain(const std::string& baseUrlNew, const std::string& portNew, bool doWePrintErrorNew, std::atomic_bool* doWeQuitNew) {
		this->timeOfLastConnection = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
		this->doWePrintError = doWePrintErrorNew;
		this->doWeQuit = doWeQuitNew;
		this->baseUrl = baseUrlNew;
		this->port = portNew;

#ifdef WIN32
		std::string certPath{ getCurrentPath() + "\\cert.pem" };
		std::string keyPath{ getCurrentPath() + "\\key.pem" };
#else
		std::string certPath{ getCurrentPath() + "/cert.pem" };
		std::string keyPath{ getCurrentPath() + "/key.pem" };
#endif

		addrinfoWrapper hints{ nullptr };
		hints->ai_family = AF_INET;
		hints->ai_socktype = SOCK_STREAM;
		hints->ai_protocol = IPPROTO_TCP;

		if (auto resultValue = getaddrinfo(baseUrlNew.c_str(), portNew.c_str(), hints, this->addrInfo); resultValue != 0) {
			if (this->doWePrintError) {
				reportError("getaddrinfo() Error: ", resultValue);
			}
			return;
		}

		if (this->theServerSocket = socket(this->addrInfo->ai_family, this->addrInfo->ai_socktype, this->addrInfo->ai_protocol); this->theServerSocket == SOCKET_ERROR) {
			if (this->doWePrintError) {
				reportError("socket() Error: ", this->theServerSocket);
			}
			return;
		}

		int32_t val{ 16384 };
		if (auto returnValue = setsockopt(this->theServerSocket, SOL_SOCKET, SO_SNDBUF, ( char* )&val, sizeof(val)); returnValue == SOCKET_ERROR) {
			if (this->doWePrintError) {
				reportError("setsockopt() Error: ", returnValue);
			}
			return;
		}

#ifdef _WIN32
		char optionValue{ true };
		if (auto returnValue = setsockopt(this->theServerSocket, IPPROTO_TCP, TCP_NODELAY, &optionValue, sizeof(optionValue)); returnValue == SOCKET_ERROR) {
			if (this->doWePrintError) {
				reportError("setsockopt() Error: ", returnValue);
			}
			return;
		}
#else
		int32_t optionValue{ 1 };
		if (auto returnValue = setsockopt(this->theServerSocket, SOL_TCP, TCP_NODELAY, &optionValue, sizeof(optionValue)); returnValue == SOCKET_ERROR) {
			if (this->doWePrintError) {
				reportError("setsockopt() Error: ", returnValue);
			}
			return;
		}
#endif

		std::unique_ptr<char> optVal{ new char{ 1 } };
		if (auto returnValue = setsockopt(this->theServerSocket, SOL_SOCKET, SO_REUSEADDR, optVal.get(), sizeof(optVal)); returnValue < 0) {
			if (this->doWePrintError) {
				reportError("setsockopt() Error: ", returnValue);
			}
			return;
		}

		if (auto returnValue = bind(this->theServerSocket, this->addrInfo->ai_addr, static_cast<int32_t>(this->addrInfo->ai_addrlen)); returnValue != 0) {
			if (this->doWePrintError) {
				reportError("bind() Error: ", returnValue);
			}
			return;
		}

		if (auto returnValue = listen(this->theServerSocket, 5); returnValue != 0) {
			if (this->doWePrintError) {
				reportError("listen() Error: ", returnValue);
			}
			return;
		}

		if (this->context = SSL_CTX_new(TLS_server_method()); this->context == nullptr) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_new() Error: ");
			}
			return;
		}

		if (SSL_CTX_use_certificate_chain_file(this->context, certPath.c_str()) <= 0) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_use_certificate_chain_file() Error: ");
			}
			return;

		}
		if (!SSL_CTX_set_min_proto_version(this->context, TLS1_2_VERSION)) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_set_min_proto_version() Error: ");
			}
			return;
		}

		if (SSL_CTX_use_PrivateKey_file(this->context, keyPath.c_str(), SSL_FILETYPE_PEM) <= 0) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_use_PrivateKey_file() Error: ");
			}
			return;
		}

		SSL_CTX_set_verify(this->context, SSL_VERIFY_PEER, NULL);
		if (!SSL_CTX_load_verify_locations(this->context, "cert.pem", NULL)) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_load_verify_locations() Error: ");
			}
			return;
		}
	};

	std::unique_ptr<WebSocketSSLShard> WebSocketSSLServerMain::connectShard(SOCKET newShard, int32_t currentShard, int32_t totalShardCount) {
		auto currentTime = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
		while (currentTime - this->timeOfLastConnection < 5000) {
			currentTime = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
		}
		std::unique_ptr<WebSocketSSLShard> theShard{ std::make_unique<WebSocketSSLShard>(newShard, this->context, this->doWePrintError, currentShard, totalShardCount) };
		this->timeOfLastConnection = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
		return theShard;
	}

	ProcessIOReturnData WebSocketSSLServerMain::processIO(std::unordered_map<SOCKET, std::unique_ptr<WebSocketSSLShard>>& theMap) noexcept {
		fd_set readSet{}, writeSet{};
		int32_t readNfds{ 0 }, writeNfds{ 0 }, finalNfds{ 0 };
		FD_ZERO(&readSet);
		FD_ZERO(&writeSet);
		for (auto& [key, value]: theMap) {
			if ((value->outputBuffer.size() > 0 || value->wantWrite) && !value->wantRead) {
				FD_SET(key, &writeSet);
			}
			FD_SET(key, &readSet);
			readNfds = key > readNfds ? key : readNfds;
			writeNfds = key > writeNfds ? key : writeNfds;
			finalNfds = readNfds > writeNfds ? readNfds : writeNfds;
		}

		timeval checkTime{ .tv_usec = 10000 };
		if (auto resultValue = select(finalNfds + 1, &readSet, &writeSet, nullptr, &checkTime); resultValue == SOCKET_ERROR) {
			if (this->doWePrintError) {
				reportError("select() Error: ", resultValue);
			}
			return ProcessIOReturnData{ .returnCode = ProcessIOReturnCode::Error, .returnIndex = 0 };
		} else if (resultValue == 0) {
			return ProcessIOReturnData{ .returnCode = ProcessIOReturnCode::Success, .returnIndex = 0 };
		}

		ProcessIOReturnData returnValue02{};
		returnValue02.returnCode = ProcessIOReturnCode::Success;
		for (auto& [key, value]: theMap) {
			if (FD_ISSET(value->clientSocket, &readSet)) {
				value->wantRead = false;
				value->wantWrite = false;
				std::string serverToClientBuffer{};
				serverToClientBuffer.resize(value->maxBufferSize);
				size_t readBytes{ 0 };
				auto returnValue{ SSL_read_ex(value->ssl, serverToClientBuffer.data(), value->maxBufferSize, &readBytes) };
				auto errorValue{ SSL_get_error(value->ssl, returnValue) };
				switch (errorValue) {
					case SSL_ERROR_NONE: {
						if (readBytes > 0) {
							value->inputBuffer.insert(value->inputBuffer.end(), serverToClientBuffer.begin(), serverToClientBuffer.begin() + readBytes);
							value->bytesRead += readBytes;
						}
						returnValue02.returnIndex = key;
						returnValue02.returnCode = ProcessIOReturnCode::Success;
						break;
					}
					case SSL_ERROR_WANT_READ: {
						returnValue02.returnCode = ProcessIOReturnCode::Success;
						value->wantRead = true;
						break;
					}
					case SSL_ERROR_WANT_WRITE: {
						returnValue02.returnCode = ProcessIOReturnCode::Success;
						value->wantWrite = true;
						break;
					}
					case SSL_ERROR_SYSCALL: {
						[[fallthrough]];
					}
					case SSL_ERROR_ZERO_RETURN: {
						[[fallthrough]];
					}
					default: {
						if (value->doWePrintError) {
							reportSSLError("WebSocketSSLServerMain::processIO::SSL_read_ex() Error: ", returnValue, value->ssl);
							reportError("WebSocketSSLServerMain::processIO::SSL_read_ex() Error: ", returnValue);
						}
						returnValue02.returnCode = ProcessIOReturnCode::Error;
						returnValue02.returnIndex = value->clientSocket;
						return returnValue02;
					}
				}
			} else if (FD_ISSET(value->clientSocket, &writeSet)) {
				value->wantRead = false;
				value->wantWrite = false;
				size_t writtenBytes{ 0 };
				std::string theString{};
				if (value->outputBuffer.size() > 0) {
					theString = std::move(value->outputBuffer.front());
				}
				size_t bytesToWrite = value->maxBufferSize < theString.size() ? value->maxBufferSize : theString.size();
				auto returnValue{ SSL_write_ex(value->ssl, theString.data(), bytesToWrite, &writtenBytes) };
				auto errorValue{ SSL_get_error(value->ssl, returnValue) };
				switch (errorValue) {
					case SSL_ERROR_NONE: {
						if (value->outputBuffer.size() > 0) {
							value->outputBuffer.erase(value->outputBuffer.begin());
						}
						value->bytesWritten += writtenBytes;
						returnValue02.returnIndex = key;
						returnValue02.returnCode = ProcessIOReturnCode::Success;
						break;
					}
					case SSL_ERROR_WANT_READ: {
						returnValue02.returnCode = ProcessIOReturnCode::Success;
						value->wantRead = true;
						break;
					}
					case SSL_ERROR_WANT_WRITE: {
						returnValue02.returnCode = ProcessIOReturnCode::Success;
						value->wantWrite = true;
						break;
					}
					case SSL_ERROR_SYSCALL: {
						[[fallthrough]];
					}
					case SSL_ERROR_ZERO_RETURN: {
						[[fallthrough]];
					}
					default: {
						if (value->doWePrintError) {
							reportSSLError("WebSocketSSLServerMain::processIO::SSL_write_ex() Error: ", returnValue, value->ssl);
							reportError("WebSocketSSLServerMain::processIO::SSL_write_ex() Error: ", returnValue);
						}
						returnValue02.returnCode = ProcessIOReturnCode::Error;
						returnValue02.returnIndex = value->clientSocket;
						return returnValue02;
					}
				}
			}
		}
		return returnValue02;
	}

	void WebSocketSSLServerMain::submitReconnectionShard(ReconnectionPackage theData) {
		std::lock_guard<std::mutex> theLock{ this->theMutex };
		this->theReconnections.push(theData);
	}

	void WebSocketSSLServerMain::reconnectShard() {
	}

	SOCKET WebSocketSSLServerMain::getNewSocket() {
		SOCKET newSocket{ SOCKET_ERROR };
#ifdef WIN32
		auto theSize = static_cast<int32_t>(this->addrInfo->ai_addrlen);
#else
		auto theSize = static_cast<uint32_t>(this->addrInfo->ai_addrlen);
#endif
		while (newSocket == SOCKET_ERROR && !this->doWeQuit->load()) {
			newSocket = accept(this->theServerSocket, this->addrInfo->ai_addr, &theSize);
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
		}

		return newSocket;
	}
}