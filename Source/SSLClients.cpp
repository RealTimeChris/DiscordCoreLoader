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
/// SSLClents.cpp - Source file for the "SSL Client" stuff.
/// May 22, 2022
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file SSLClients.cpp

#include <discordcoreloader/SSLClients.hpp>
#include <discordcoreloader/WebSocketEntities.hpp>

namespace DiscordCoreLoader {

	std::atomic_int64_t theValue{};

	std::string getCurrentPath() {
		std::stringstream theStream{};
		theStream << std::filesystem::current_path();
		return std::string{ theStream.str().substr(1, theStream.str().size() - 2) };
	}

	std::string reportSSLError(const std::string& errorPosition, int32_t errorValue = 0, SSL* ssl = nullptr) noexcept {
		std::stringstream stream{};
		stream << errorPosition << " Error: ";
		if (ssl) {
			stream << SSL_get_error(ssl, errorValue) << ", " << ERR_error_string(errorValue, nullptr) << std::endl << std::endl;
		} else {
			stream << ERR_error_string(errorValue, nullptr) << std::endl << std::endl;
		}
		return std::string{ stream.str() };
	}

	std::string reportError(const char* errorPosition, int32_t value) noexcept {
		std::stringstream stream{};
		stream << errorPosition << " Error: ";
#ifdef _WIN32
		char string[1024]{};
	#ifdef UWP
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), ( LPWSTR )string, 1024, NULL);
	#else
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), string, 1024, NULL);
	#endif
		stream << WSAGetLastError() << ", " << string << std::endl;
#else
		stream << strerror(errno) << DiscordCoreLoader::reset();
#endif
		return std::string{ stream.str() };
	}

#ifdef _WIN32
	void WSADataWrapper::WSADataDeleter::operator()(WSADATA* other) {
		WSACleanup();
		delete other;
	}

	WSADataWrapper::WSADataWrapper() {
		auto returnValue = WSAStartup(MAKEWORD(2, 2), this->ptr.get());
		if (returnValue) {
		}
	}
#endif

	void SSL_CTXWrapper::SSL_CTXDeleter::operator()(SSL_CTX* other) {
		if (other) {
			SSL_CTX_free(other);
			other = nullptr;
		}
	}

	SSL_CTXWrapper& SSL_CTXWrapper::operator=(SSL_CTX* other) {
		this->ptr.reset(nullptr);
		this->ptr = std::unique_ptr<SSL_CTX, SSL_CTXDeleter>(other, SSL_CTXDeleter{});
		return *this;
	}

	SSL_CTXWrapper::operator SSL_CTX*() {
		return this->ptr.get();
	}

	void SSLWrapper::SSLDeleter::operator()(SSL* other) {
		if (other) {
			SSL_shutdown(other);
			SSL_free(other);
			other = nullptr;
		}
	}

	SSLWrapper& SSLWrapper::operator=(nullptr_t other) {
		this->ptr.reset(other);
		return *this;
	}

	SSLWrapper& SSLWrapper::operator=(SSL* other) {
		this->ptr.reset(nullptr);
		this->ptr = std::unique_ptr<SSL, SSLDeleter>(other, SSLDeleter{});
		return *this;
	}

	SSLWrapper::operator SSL*() {
		return this->ptr.get();
	}

	void SOCKETWrapper::SOCKETDeleter::operator()(SOCKET* other) {
		if (*other != SOCKET_ERROR) {
#ifdef _WIN32
			shutdown(*other, SD_BOTH);
			closesocket(*other);
#else
			shutdown(*other, SHUT_RDWR);
			close(*other);
#endif
			*other = SOCKET_ERROR;
		};
	};

	SOCKETWrapper& SOCKETWrapper::operator=(SOCKETWrapper&& other) noexcept {
		this->ptr.reset(nullptr);
		this->ptr  = std::unique_ptr<SOCKET, SOCKETDeleter>(new SOCKET{}, SOCKETDeleter{});
		*this->ptr = *other.ptr;
		*other.ptr = SOCKET_ERROR;
		return *this;
	}

	SOCKETWrapper::SOCKETWrapper(SOCKETWrapper&& other) noexcept {
		*this = std::move(other);
	}

	SOCKETWrapper& SOCKETWrapper::operator=(SOCKET other) noexcept {
		this->ptr.reset(nullptr);
		this->ptr  = std::unique_ptr<SOCKET, SOCKETDeleter>(new SOCKET{}, SOCKETDeleter{});
		*this->ptr = other;
		return *this;
	}

	SOCKETWrapper::SOCKETWrapper(SOCKET other) noexcept {
		*this = other;
	}

	SOCKETWrapper::operator SOCKET*() noexcept {
		return this->ptr.get();
	}

	SOCKETWrapper::operator SOCKET() noexcept {
		return *this->ptr;
	}

	sockaddr* sockaddrWrapper::operator->() {
		return reinterpret_cast<sockaddr*>(&this->ptr);
	}

	sockaddrWrapper::operator sockaddr_in*() {
		return &this->ptr;
	}

	sockaddrWrapper::operator sockaddr*() {
		return reinterpret_cast<sockaddr*>(&this->ptr);
	}

	addrinfo* addrinfoWrapper::operator->() {
		return this->ptr;
	}

	addrinfoWrapper::operator addrinfo**() {
		this->doWeClearAddrInfo = true;
		return &this->ptr;
	}

	addrinfoWrapper::operator addrinfo*() {
		return this->ptr;
	}

	addrinfoWrapper::~addrinfoWrapper() {
		if (this->doWeClearAddrInfo) {
			freeaddrinfo(this->ptr);
		} else {
			delete this->ptr;
		}
	}

	SSLClient& SSLClient::operator=(SSLClient&& other) noexcept {
		if (this != &other) {
			this->serverToClientBuffer	= std::move(other.serverToClientBuffer);
			this->theCurrentMessage		= std::move(other.theCurrentMessage);
			this->currentReconnectTries = other.currentReconnectTries;
			this->theMessageQueue		= std::move(other.theMessageQueue);
			this->currentSocketIndex	= other.currentSocketIndex;
			this->outputBuffers			= std::move(other.outputBuffers);
			this->clientSocket			= std::move(other.clientSocket);
			this->currentGuildCount		= other.currentGuildCount;
			this->inputBuffer			= std::move(other.inputBuffer);
			this->theContext			= std::move(other.theContext);
			this->totalGuildCount		= other.totalGuildCount;
			this->areWeConnected		= other.areWeConnected;
			this->doWePrintError		= other.doWePrintError;
			this->lastNumberSent		= other.lastNumberSent;
			this->authKey				= std::move(other.authKey);
			this->bytesWritten			= other.bytesWritten;
			this->theOpCode				= other.theOpCode;
			this->sendGuilds			= other.sendGuilds;
			this->bytesRead				= other.bytesRead;
			this->ssl					= std::move(other.ssl);
			this->theState				= other.theState;
			this->shard[0]				= other.shard[0];
			this->shard[1]				= other.shard[1];
		}
		return *this;
	}

	SSLClient::SSLClient(SSLClient&& other) noexcept {
		*this = std::move(other);
	}

	SSLClient::SSLClient(SOCKET theSocket, SSL_CTX* theContextNew, bool doWePrintErrorsNew) : maxBufferSize(1024 * 16) {
		this->serverToClientBuffer.resize(this->maxBufferSize);
		this->doWePrintError = doWePrintErrorsNew;
		this->theContext	 = theContextNew;
		this->clientSocket	 = theSocket;

		if (this->clientSocket != SOCKET_ERROR) {
			if (this->ssl = SSL_new(this->theContext); this->ssl == nullptr) {
				if (this->doWePrintError) {
					std::cout << reportSSLError("SSL_new() Error: ");
				}
				return;
			}

			if (!SSL_set_min_proto_version(this->ssl, TLS1_2_VERSION)) {
				if (this->doWePrintError) {
					std::cout << reportSSLError("SSL_set_min_proto_version() Error: ");
				}
				return;
			}

			if (auto returnValue = SSL_set_fd(this->ssl, this->clientSocket); !returnValue) {
				if (this->doWePrintError) {
					std::cout << reportSSLError("SSL_set_fd() Error: ", returnValue, this->ssl);
				}
				return;
			}

			while (SSL_accept(this->ssl) != 1) {
				std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
			}

			this->areWeConnected = true;
		}
	};

	void SSLClient::writeData(std::string& dataToWrite, bool priority) noexcept {
		if (dataToWrite.size() > 0 && this->ssl) {
			if (priority && dataToWrite.size() < static_cast<size_t>(16 * 1024)) {
				pollfd readWriteSet{};
				readWriteSet.fd		= this->clientSocket;
				readWriteSet.events = POLLOUT;
				if (auto returnValue = poll(&readWriteSet, 1, 1); returnValue == SOCKET_ERROR) {
					this->disconnect();
					return;
				} else if (returnValue == 0) {
					return;
				}

				if (readWriteSet.revents & POLLOUT) {
					this->outputBuffers.emplace_back(dataToWrite);
					if (!this->writeDataProcess()) {
						return;
					}
				}
			} else {
				if (dataToWrite.size() >= static_cast<size_t>(16 * 1024)) {
					size_t remainingBytes{ dataToWrite.size() };
					while (remainingBytes > 0) {
						std::string newString{};
						size_t amountToCollect{};
						if (dataToWrite.size() >= static_cast<size_t>(1024 * 16)) {
							amountToCollect = static_cast<size_t>(1024 * 16);
						} else {
							amountToCollect = dataToWrite.size();
						}
						newString.insert(newString.begin(), dataToWrite.begin(), dataToWrite.begin() + amountToCollect);
						this->outputBuffers.emplace_back(newString);
						dataToWrite.erase(amountToCollect);
						remainingBytes = dataToWrite.size();
					}
				} else {
					this->outputBuffers.emplace_back(dataToWrite);
				}
			}
		}
		return;
	}

	std::string& SSLClient::getInputBuffer() noexcept {
		return this->inputBuffer;
	}

	bool SSLClient::areWeStillConnected() noexcept {
		if (this->clientSocket != SOCKET_ERROR) {
			return true;
		} else {
			return false;
		}
	}

	uint64_t SSLClient::getBytesRead() noexcept {
		return this->bytesRead;
	}

	bool SSLClient::writeDataProcess() noexcept {
		if (this->outputBuffers.size() > 0) {
			size_t writtenBytes{ 0 };
			auto returnValue{ SSL_write_ex(this->ssl, this->outputBuffers.front().data(), this->outputBuffers.front().size(), &writtenBytes) };
			auto errorValue{ SSL_get_error(this->ssl, returnValue) };
			switch (errorValue) {
				case SSL_ERROR_WANT_READ: {
					[[fallthrough]];
				}
				case SSL_ERROR_WANT_WRITE: {
					[[fallthrough]];
				}
				case SSL_ERROR_NONE: {
					if (writtenBytes > 0) {
						this->outputBuffers.erase(this->outputBuffers.begin());
					}
					return true;
				}
				case SSL_ERROR_ZERO_RETURN: {
					std::cout << reportSSLError("SSLClient::writeDataProcess()");
					[[fallthrough]];
				}
				default: {
					std::cout << reportSSLError("SSLClient::writeDataProcess()");
					this->disconnect();
					return false;
				}
			}
		}
		return true;
	}

	bool SSLClient::readDataProcess() noexcept {
		do {
			size_t readBytes{ 0 };
			auto returnValue{ SSL_read_ex(this->ssl, this->rawInputBuffer.data(), this->maxBufferSize, &readBytes) };
			auto errorValue{ SSL_get_error(this->ssl, returnValue) };
			switch (errorValue) {
				case SSL_ERROR_WANT_READ: {
					[[fallthrough]];
				}
				case SSL_ERROR_WANT_WRITE: {
					[[fallthrough]];
				}
				case SSL_ERROR_NONE: {
					if (readBytes > 0) {
						this->inputBuffer.append(this->rawInputBuffer.data(), readBytes);
						this->bytesRead += readBytes;
					}
					break;
				}
				case SSL_ERROR_ZERO_RETURN: {
					std::cout << reportSSLError("SSLClient::readDataProcess()");
					break;
				}
				default: {
					std::cout << reportSSLError("SSLClient::readDataProcess()");
					this->disconnect();
					return false;
				}
			}
			this->handleBuffer();
		} while (SSL_pending(this->ssl));
		return true;
	}

	void SSLClient::disconnect() noexcept {
		if (this->clientSocket != SOCKET_ERROR) {
			this->clientSocket = SOCKET_ERROR;
			this->outputBuffers.clear();
			this->inputBuffer.clear();
		}
	}

	SSLClient::~SSLClient() {
		this->areWeConnected = false;
	}

	WebSocketSSLServerMain::WebSocketSSLServerMain(const std::string& baseUrlNew, const std::string& portNew, bool doWePrintErrorNew, std::atomic_bool* doWeQuitNew,
		ConfigParser* theData) {
		this->doWePrintError  = doWePrintErrorNew;
		this->theConfigParser = theData;
		this->doWeQuit		  = doWeQuitNew;
		this->baseUrl		  = baseUrlNew;
		this->port			  = portNew;

#ifdef _WIN32
		std::string certPath{ getCurrentPath() + "\\Cert.pem" };
		std::string keyPath{ getCurrentPath() + "\\Key.pem" };
#elif __linux__
		std::string certPath{ getCurrentPath() + "/Cert.pem" };
		std::string keyPath{ getCurrentPath() + "/Key.pem" };
#endif

		addrinfoWrapper hints{};
		hints->ai_family   = AF_INET;
		hints->ai_socktype = SOCK_STREAM;
		hints->ai_protocol = IPPROTO_TCP;

		if (auto resultValue = getaddrinfo(this->baseUrl.data(), this->port.data(), hints, this->addrInfo); resultValue != 0) {
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
		/*
		if (!SSL_CTX_set_min_proto_version(this->context, TLS1_2_VERSION)) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_set_min_proto_version() Error: ");
			}
			return;
		}
		*/
		if (!SSL_CTX_set_ciphersuites(this->context, "TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256")) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_set_ciphersuites() Error: ");
			}
			return;
		}

		if (SSL_CTX_use_certificate_chain_file(this->context, certPath.data()) <= 0) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_use_certificate_chain_file() Error: ");
			}
			return;
		}

		if (SSL_CTX_use_PrivateKey_file(this->context, keyPath.data(), SSL_FILETYPE_PEM) <= 0) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_use_PrivateKey_file() Error: ");
			}
			return;
		}

		SSL_CTX_set_verify(this->context, SSL_VERIFY_PEER, NULL);
		if (!SSL_CTX_load_verify_locations(this->context, "Cert.pem", NULL)) {
			if (this->doWePrintError) {
				reportSSLError("SSL_CTX_load_verify_locations() Error: ");
			}
			return;
		}
	};

	std::vector<WebSocketSSLShard*> WebSocketSSLServerMain::processIO(std::vector<WebSocketSSLShard*>& theVector) noexcept {
		PollFDWrapper readWriteSet{};
		for (uint32_t x = 0; x < theVector.size(); ++x) {
			pollfd theWrapper{};
			theWrapper.fd = theVector[x]->clientSocket;
			if (theVector[x]->outputBuffers.size() > 0) {
				theWrapper.events = POLLIN | POLLOUT;
			} else {
				theWrapper.events = POLLIN;
			}
			readWriteSet.theIndices.emplace_back(x);
			readWriteSet.thePolls.emplace_back(theWrapper);
		}

		std::vector<WebSocketSSLShard*> returnValue02{};
		if (readWriteSet.theIndices.size() == 0) {
			return returnValue02;
		}

		if (auto returnValue = poll(readWriteSet.thePolls.data(), static_cast<unsigned long>(readWriteSet.theIndices.size()), 1); returnValue == SOCKET_ERROR) {
			for (uint32_t x = 0; x < readWriteSet.thePolls.size(); ++x) {
				if (readWriteSet.thePolls[x].revents & POLLERR || readWriteSet.thePolls[x].revents & POLLHUP || readWriteSet.thePolls[x].revents & POLLNVAL) {
					returnValue02.emplace_back(theVector[readWriteSet.theIndices[x]]);
				}
			}
			return returnValue02;

		} else if (returnValue == 0) {
			return returnValue02;
		}

		for (uint32_t x = 0; x < readWriteSet.theIndices.size(); ++x) {
			if (readWriteSet.thePolls[x].revents & POLLOUT) {
				if (!theVector[readWriteSet.theIndices[x]]->writeDataProcess()) {
					returnValue02.emplace_back(theVector[readWriteSet.theIndices[x]]);
					continue;
				}
			}
			if (readWriteSet.thePolls[x].revents & POLLIN) {
				if (!theVector[readWriteSet.theIndices[x]]->readDataProcess()) {
					returnValue02.emplace_back(theVector[readWriteSet.theIndices[x]]);
					continue;
				}
			}
			theVector[readWriteSet.theIndices[x]]->handleBuffer();
		}
		return returnValue02;
	}

	SOCKET WebSocketSSLServerMain::getNewSocket() {
		SOCKET newSocket{ static_cast<SOCKET>(SOCKET_ERROR) };
#ifdef _WIN32
		auto theSize = static_cast<int32_t>(this->addrInfo->ai_addrlen);
#else
		auto theSize = static_cast<uint32_t>(this->addrInfo->ai_addrlen);
#endif
		while (newSocket == SOCKET_ERROR && !this->doWeQuit->load()) {
			newSocket = accept(this->theServerSocket, this->addrInfo->ai_addr, &theSize);
			std::this_thread::sleep_for(std::chrono::milliseconds{ 5 });
			const char optionValue{ true };
			if (setsockopt(newSocket, IPPROTO_TCP, TCP_NODELAY, &optionValue, sizeof(int32_t))) {
				if (this->doWePrintError) {
					reportError("SSLClient::connect::setsockopt()", newSocket);
				}
				return SOCKET_ERROR;
			}
#ifdef _WIN32
			u_long value02{ 1 };
			if (ioctlsocket(newSocket, FIONBIO, &value02)) {
				return SOCKET_ERROR;
			}
#else
			if (fcntl(newSocket, F_SETFL, fcntl(newSocket, F_GETFL, 0) | O_NONBLOCK)) {
				return SOCKET_ERROR;
			}
#endif

			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
		}
		return newSocket;
	}
}