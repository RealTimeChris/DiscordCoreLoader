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
/// WebSocketEntities.cpp - Source file for the webSocket related classes and structs.
/// May 22, 2022
/// https://discordcoreapi.com
/// \file WebSocketEntities.cpp

#include <discordcoreloader/WebSocketEntities.hpp>
#include <discordcoreloader/DiscordCoreClient.hpp>

namespace DiscordCoreLoader {

	constexpr uint16_t webSocketMaxPayloadLengthLarge{ 65535 };
	constexpr uint8_t webSocketPayloadLengthMagicLarge{ 126 };
	constexpr uint8_t webSocketPayloadLengthMagicHuge{ 127 };
	constexpr uint8_t maxHeaderSize{ sizeof(uint64_t) + 2 };
	constexpr uint8_t webSocketMaxPayloadLengthSmall{ 125 };
	constexpr uint8_t webSocketFinishBit{ (1u << 7u) };
	constexpr uint8_t webSocketMaskBit{ (1u << 7u) };

	BaseSocketAgent::BaseSocketAgent(WebSocketSSLServerMain* webSocketSSLServerMainNew, DiscordCoreClient* discordCoreClient, std::atomic_bool* doWeQuitNew) noexcept {
		this->jsonifier = discordCoreClient->configParser.getTheData();
		this->webSocketSSLServerMain = webSocketSSLServerMainNew;
		this->discordCoreClient = discordCoreClient;
		this->state = WebSocketState::Initializing;
		this->doWeQuit = doWeQuitNew;
		this->theTask = std::make_unique<std::jthread>([this](std::stop_token theToken) {
			this->run(theToken);
		});
	}

	void BaseSocketAgent::sendMessage(std::string* dataToSend, SOCKET theIndex) noexcept {
		try {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessSentMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				std::cout << shiftToBrightBlue() << "Sending WebSocket " + this->theClients[theIndex]->shard.dump() + std::string("'s Message: ") << std::endl
						  << *dataToSend << reset();
			}

			if (this->webSocketSSLServerMain != nullptr) {
				this->theClients[theIndex]->writeData(*dataToSend);
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::sendMessage()");
			}
			this->handleDroppedConnection(theIndex);
		}
	}

	std::jthread* BaseSocketAgent::getTheTask() noexcept {
		return this->theTask.get();
	}

	void BaseSocketAgent::sendMessage(nlohmann::json& dataToSend, WebSocketOpCode theOpCode, SOCKET theIndex) noexcept {
		try {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessSentMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				std::cout << shiftToBrightBlue() << "Sending WebSocket " + this->theClients[theIndex]->shard.dump() + std::string("'s Message: ") << dataToSend.dump() << reset()
						  << reset() << std::endl
						  << std::endl;
			}
			uint64_t theHeaderLength{};
			std::string theVector{};
			if (this->theMode == DiscordCoreLoader::WebSocketMode::ETF) {
				theVector = this->erlPacker.parseJsonToEtf(std::move(dataToSend));
			} else {
				theVector = dataToSend.dump();
			}
			if (theVector.size() <= webSocketMaxPayloadLengthSmall) {
				theHeaderLength = 2;
			} else if (theVector.size() <= webSocketMaxPayloadLengthLarge) {
				theHeaderLength = 4;
			} else {
				theHeaderLength = 10;
			}
			uint64_t totalLength{ theVector.size() };
			std::vector<std::string> theStrings{};
			if (theVector.size() > (static_cast<uint64_t>(16) * 1024) - theHeaderLength) {
				uint64_t incrementAmount{ (static_cast<uint64_t>(16) * 1024) - theHeaderLength };
				uint64_t remainingBytes{ theVector.size() };
				while (remainingBytes > 0) {
					std::string theString{};
					theString.insert(theString.begin(), theVector.end() - remainingBytes, theVector.end() - remainingBytes + incrementAmount);
					remainingBytes -= incrementAmount;
					theStrings.push_back(std::move(theString));
					if (remainingBytes < (static_cast<uint64_t>(16) * 1024) - theHeaderLength) {
						incrementAmount = remainingBytes;
					}
				}

			} else {
				std::string theString{};
				theString.insert(theString.begin(), theVector.begin(), theVector.end());
				totalLength = theString.size();
				theStrings.push_back(std::move(theString));
			}
			for (uint64_t x = 0; x < static_cast<uint64_t>(theStrings.size()); x += 1) {
				std::string header{};
				if (x == 0) {
					this->createHeader(header, totalLength, theOpCode, true, theIndex);
				}
				std::string theVectorNew{};
				theVectorNew.insert(theVectorNew.begin(), header.begin(), header.end());
				theVectorNew.insert(theVectorNew.begin() + header.size(), theStrings[x].begin(), theStrings[x].end());
				this->theClients[theIndex]->writeData(theVectorNew);
			}

		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::sendMessage()");
			}
			this->handleDroppedConnection(theIndex);
		}
	}

	uint64_t BaseSocketAgent::createHeader(std::string& outBuffer, uint64_t sendLength, WebSocketOpCode opCodeNew, bool isItFinal, SOCKET theIndex) noexcept {
		try {
			if (isItFinal) {
				outBuffer.push_back(static_cast<uint8_t>(opCodeNew) | webSocketFinishBit);
			} else {
				outBuffer.push_back(static_cast<uint8_t>(opCodeNew));
			}

			uint32_t indexCount{ 0 };
			if (sendLength <= webSocketMaxPayloadLengthSmall) {
				outBuffer.push_back(static_cast<uint8_t>(sendLength));
				indexCount = 0;
			} else if (sendLength <= webSocketMaxPayloadLengthLarge) {
				outBuffer.push_back(static_cast<uint8_t>(webSocketPayloadLengthMagicLarge));
				indexCount = 2;
			} else {
				outBuffer.push_back(static_cast<uint8_t>(webSocketPayloadLengthMagicHuge));
				indexCount = 8;
			}
			for (int32_t x = indexCount - 1; x >= 0; x--) {
				outBuffer.push_back(static_cast<uint8_t>(sendLength >> x * 8));
			}

			outBuffer[1] |= 0 << 7;
			return outBuffer.size();
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::createHeader()");
			}
			this->handleDroppedConnection(theIndex);
			return uint64_t{};
		}
	}

	void BaseSocketAgent::sendCreateGuilds(SOCKET theIndex) noexcept {
		if (this->theClients[theIndex]->currentGuildCount < this->theClients[theIndex]->totalGuildCount) {
			this->theClients[theIndex]->currentGuildCount += 1;
			this->theClients[theIndex]->lastNumberSent += 1;
			auto theGuildNew = this->discordCoreClient->theGuildHolder;
			theGuildNew["s"] = this->theClients[theIndex]->lastNumberSent;
			WebSocketMessage theMessage{};
			theMessage.jsonMsg = std::move(theGuildNew);
			if (this->theMode == WebSocketMode::ETF) {
				theMessage.theOpCode = WebSocketOpCode::Op_Binary;
			} else {
				theMessage.theOpCode = WebSocketOpCode::Op_Text;
			}
			this->theClients[theIndex]->theMessageQueue.push(std::move(theMessage));
		}
	}

	void BaseSocketAgent::sendReadyMessage(SOCKET theIndex) noexcept {
		this->theClients[theIndex]->lastNumberSent += 1;
		nlohmann::json jsonData{};
		jsonData["op"] = static_cast<int8_t>(0);
		jsonData["s"] = this->theClients[theIndex]->lastNumberSent;
		jsonData["t"] = "READY";
		jsonData["d"]["session_id"] = this->jsonifier.randomizeId();
		jsonData["d"]["guilds"];
		int32_t guildSize = this->discordCoreClient->guildQuantity / this->discordCoreClient->shardingOptions.totalNumberOfShards;
		this->theClients[theIndex]->totalGuildCount = guildSize;
		if (guildSize > 2500) {
			this->initDisconnect(WebSocketCloseCode::Sharding_Required, theIndex);
			return;
		}
		for (int32_t x = 0; x < this->discordCoreClient->guildQuantity / this->discordCoreClient->shardingOptions.totalNumberOfShards; x += 1) {
			auto theDifference = UINT64_MAX / this->discordCoreClient->shardingOptions.totalNumberOfShards;
			auto minValue = static_cast<uint64_t>(theDifference) * static_cast<uint64_t>(this->theClients[theIndex]->shard[0]);
			auto maxValue = minValue + theDifference;
			auto theGuild = this->jsonifier.generateUnavailableGuild(minValue, maxValue);
			jsonData["d"]["guilds"].push_back(this->jsonifier.JSONIFYUnavailableGuild(theGuild));
			this->theGuilds[theIndex].push_back(theGuild);
		}
		jsonData["d"]["v"] = 10;
		jsonData["d"]["user"] = this->jsonifier.JSONIFYUser(std::move(*this->jsonifier.generateUser()));
		WebSocketMessage theMessage{};
		theMessage.jsonMsg = std::move(jsonData);
		if (this->theMode == WebSocketMode::ETF) {
			theMessage.theOpCode = WebSocketOpCode::Op_Binary;
		} else {
			theMessage.theOpCode = WebSocketOpCode::Op_Text;
		}

		std::queue<WebSocketMessage> theMessageQueue{};
		theMessageQueue.push(std::move(theMessage));
		for (uint32_t x = 0; x < this->theClients[theIndex]->theMessageQueue.size(); x += 1) {
			theMessageQueue.push(std::move(this->theClients[theIndex]->theMessageQueue.front()));
			this->theClients[theIndex]->theMessageQueue.pop();
		}
		this->theClients[theIndex]->theMessageQueue = std::move(theMessageQueue);
	}

	void BaseSocketAgent::sendHelloMessage(SOCKET theIndex) noexcept {
		nlohmann::json jsonData{};
		jsonData["op"] = static_cast<int8_t>(10);
		jsonData["s"];
		jsonData["t"];
		jsonData["d"]["heartbeat_interval"] = static_cast<uint16_t>(this->heartbeatInterval);
		jsonData["d"]["_trace"] = nlohmann::json::array();
		jsonData["d"]["_trace"].push_back(std::string({ "[\"gateway-prd-main-6dtt\",{\"micros\":0.0}]" }));
		WebSocketMessage theMessage{};
		theMessage.jsonMsg = std::move(jsonData);
		if (this->theMode == WebSocketMode::ETF) {
			theMessage.theOpCode = WebSocketOpCode::Op_Binary;
		} else {
			theMessage.theOpCode = WebSocketOpCode::Op_Text;
		}

		std::queue<WebSocketMessage> theMessageQueue{};
		theMessageQueue.push(std::move(theMessage));
		for (uint32_t x = 0; x < this->theClients[theIndex]->theMessageQueue.size(); x += 1) {
			theMessageQueue.push(std::move(this->theClients[theIndex]->theMessageQueue.front()));
			this->theClients[theIndex]->theMessageQueue.pop();
		}
		this->theClients[theIndex]->theMessageQueue = std::move(theMessageQueue);
	}

	void BaseSocketAgent::sendHeartBeat(SOCKET theIndex) noexcept {
		nlohmann::json jsonData{};
		jsonData["op"] = static_cast<int8_t>(11);
		jsonData["s"];
		jsonData["t"];
		jsonData["d"];
		WebSocketMessage theMessage{};
		theMessage.jsonMsg = std::move(jsonData);
		if (this->theMode == WebSocketMode::ETF) {
			theMessage.theOpCode = WebSocketOpCode::Op_Binary;
		} else {
			theMessage.theOpCode = WebSocketOpCode::Op_Text;
		}
		std::queue<WebSocketMessage> theMessageQueue{};
		theMessageQueue.push(std::move(theMessage));
		for (uint32_t x = 0; x < this->theClients[theIndex]->theMessageQueue.size(); x += 1) {
			theMessageQueue.push(std::move(this->theClients[theIndex]->theMessageQueue.front()));
			this->theClients[theIndex]->theMessageQueue.pop();
		}
		this->theClients[theIndex]->theMessageQueue = std::move(theMessageQueue);
	}

	std::vector<std::string> BaseSocketAgent::tokenize(const std::string& dataIn, const std::string& separator, SOCKET theIndex) noexcept {
		try {
			std::string::size_type value{ 0 };
			std::vector<std::string> dataOut{};
			while ((value = dataIn.find_first_not_of(separator, value)) != std::string::npos) {
				auto output = dataIn.find(separator, value);
				dataOut.push_back(dataIn.substr(value, output - value));
				if (dataIn.substr(value, output - value).find("GET") != std::string::npos) {
					std::string newString = dataIn.substr(dataIn.find("v=") + 2, dataIn.length() - dataIn.find("v=") + 2);
					int8_t index{ 0 };
					for (auto& value02: newString) {
						index += 1;
						if (value != isdigit(value02)) {
							index += 1;
							break;
						}
					}

					int32_t version = stoi(newString.substr(0, index));
					if (version != 10) {
						this->initDisconnect(WebSocketCloseCode::Invalid_API_Version, theIndex);
					}
				}
				if (dataIn.substr(value, output - value).find("Sec-WebSocket-Key:") != std::string::npos) {
					this->authKey = dataOut[dataOut.size() - 1].substr(dataOut[dataOut.size() - 1].find("Sec-WebSocket-Key: ") + std::string{ "Sec-WebSocket-Key: " }.size());
				}
				if (dataIn.substr(value, output - value).find("json") != std::string::npos) {
					this->theMode = WebSocketMode::JSON;
				} else if (dataIn.substr(value, output - value).find("etf") != std::string::npos) {
					this->theMode = WebSocketMode::ETF;
				}
				value = output;
			}
			return dataOut;
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::tokenize()");
			}
			this->handleDroppedConnection(theIndex);
			return std::vector<std::string>{};
		}
	}

	void BaseSocketAgent::respondToDisconnect(SOCKET theIndex) noexcept {
		std::string theString{};
		theString.push_back(static_cast<int8_t>(WebSocketOpCode::Op_Close) | static_cast<int8_t>(webSocketFinishBit));
		theString.push_back(0);
		theString.push_back(static_cast<int8_t>(static_cast<uint16_t>(1000) >> 8));
		theString.push_back(static_cast<int8_t>(1000 & 0xff));
		if (this->webSocketSSLServerMain != nullptr) {
			this->theClients[theIndex]->writeData(theString);
		}
		if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
			std::cout << shiftToBrightRed() << "WebSocket " + this->theClients[theIndex]->shard.dump() + " Closed; Code: " << this->closeCode << reset() << std::endl;
		}

		this->closeCode = 0;
		if ((this->maxReconnectTries > this->currentReconnectTries)) {
			this->currentReconnectTries += 1;

			while (this->theClients[theIndex]->theMessageQueue.size() > 0) {
				this->theClients[theIndex]->theMessageQueue.pop();
			}
			this->handleDroppedConnection(theIndex);
		} else {
			this->doWeQuit->store(true);
			this->theTask->request_stop();
		}
	}

	void BaseSocketAgent::initDisconnect(WebSocketCloseCode reason, SOCKET theIndex) noexcept {
		std::string theString{};
		theString.push_back(static_cast<int8_t>(WebSocketOpCode::Op_Close) | static_cast<int8_t>(webSocketFinishBit));
		theString.push_back(0);
		theString.push_back(static_cast<uint16_t>(reason) >> 8);
		theString.push_back(static_cast<int8_t>(reason) & 0xff);

		while (this->theClients[theIndex]->theMessageQueue.size() > 0) {
			this->theClients[theIndex]->theMessageQueue.pop();
		}
		WebSocketMessage theMessage{};
		theMessage.stringMsg = theString;
		theMessage.theOpCode = WebSocketOpCode::Op_Close;
		this->theClients[theIndex]->theMessageQueue.push(theMessage);
	}

	void BaseSocketAgent::handleDroppedConnection(SOCKET theIndex) noexcept {
		if (this->currentReconnectTries <= this->maxReconnectTries) {
			this->currentReconnectTries += 1;
			ReconnectionPackage theData{};
			theData.currentShard = this->theClients[theIndex]->shard[0];
			theData.totalShardCount = this->theClients[theIndex]->shard[1];
			theData.theMap = &this->theClients;
			theData.theSocket = this->theClients[theIndex]->clientSocket;
			this->webSocketSSLServerMain->submitReconnectionShard(theData);
		}
	}

	void BaseSocketAgent::sendFinalMessage(SOCKET theIndex) noexcept {
		if (this->theClients[theIndex]->outputBuffer.size() == 0) {
			if (this->theClients[theIndex]->theMessageQueue.size() > 0) {
				WebSocketMessage newMessage = std::move(this->theClients[theIndex]->theMessageQueue.front());
				this->theClients[theIndex]->theMessageQueue.pop();
				if (newMessage.stringMsg.size() > 0) {
					this->sendMessage(&newMessage.stringMsg, this->theClients[theIndex]->clientSocket);
				} else {
					if (this->theMode == WebSocketMode::ETF) {
						this->sendMessage(newMessage.jsonMsg, WebSocketOpCode::Op_Binary, this->theClients[theIndex]->clientSocket);
					} else {
						this->sendMessage(newMessage.jsonMsg, WebSocketOpCode::Op_Text, this->theClients[theIndex]->clientSocket);
					}
				}
			}
		}
	}

	void BaseSocketAgent::run(std::stop_token theToken) noexcept {
		try {
			if (this->theGuilds.size() > 2500) {
				this->initDisconnect(WebSocketCloseCode::Sharding_Required, this->currentNewSocket);
				this->theGuilds.clear();
			}
			while (!theToken.stop_requested() && !this->doWeQuit->load()) {
				if (this->doWeConnect.load()) {
					this->connectInternal();
				}
				if (this->theClients.size() > 0) {
					ProcessIOReturnData returnValue{};
					do {
						returnValue = this->webSocketSSLServerMain->processIO(this->theClients);
						if (returnValue.returnCode == ProcessIOReturnCode::Error) {
							this->respondToDisconnect(returnValue.returnIndex);
						}
					} while (returnValue.writtenOrReadCount != 0);
					for (auto& [key, value]: this->theClients) {
						this->handleBuffer(key);
						if (value->outputBuffer.size() == 0) {
							if (value->sendGuilds) {
								this->sendCreateGuilds(key);
							}
						}
						this->sendFinalMessage(key);
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
			}
			if (this->opCode != WebSocketOpCode::Op_Close) {
				this->initDisconnect(static_cast<WebSocketCloseCode>(1000), this->currentIndex);
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::run()");
			}
			this->handleDroppedConnection(this->currentIndex);
		}
	}

	void BaseSocketAgent::sendGuildMemberChunks() noexcept {
	}

	void BaseSocketAgent::onMessageReceived(SOCKET theIndex) noexcept { 
		try {
			std::string messageNew{};

			messageNew = std::move(this->theClients[theIndex]->getInputBuffer());
			nlohmann::json payload{};

			if (this->theMode == DiscordCoreLoader::WebSocketMode::ETF) {
				try {
					payload = this->erlPacker.parseEtfToJson(&messageNew);
				} catch (...) {
					DiscordCoreLoader::reportException("ErlPacker::parseEtfToJson()");
					return;
				}
			} else {
				payload = nlohmann::json::parse(messageNew);
			}
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessReceiveMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				std::cout << shiftToBrightGreen() << "Message received from WebSocket " + this->theClients[theIndex]->shard.dump() + ": " << payload.dump() << reset() << std::endl
						  << std::endl;
			}

			if (payload["op"] == 8) {
				this->sendGuildMemberChunks();
			}

			if (payload["op"] == 1) {
				this->sendHeartBeat(theIndex);
				this->theClients[theIndex]->getInputBuffer().clear();
			}

			if (payload["op"] == 2) {
				if (payload["d"].contains("shard")) {
					this->theClients[theIndex]->shard = payload["d"]["shard"];
					this->intentsValue = payload["d"]["intents"];
				}
				if (!this->discordCoreClient->haveWeCollectedShardingInfo) {
					this->discordCoreClient->shardingOptions.totalNumberOfShards = this->theClients[theIndex]->shard[1];
					this->discordCoreClient->haveWeCollectedShardingInfo = true;
					this->discordCoreClient->shardingOptions.startingShard = this->theClients[theIndex]->shard[0];
				}
				this->sendReadyMessage(theIndex);
				this->theClients[theIndex]->getInputBuffer().clear();
				this->theClients[theIndex]->sendGuilds = true;
			}
			return;
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::onMessageReceived()");
			}
			this->handleDroppedConnection(theIndex);
			return;
		}
	}

	void BaseSocketAgent::handleBuffer(SOCKET theIndex) noexcept {
		try {
			std::string newVector = this->theClients[theIndex]->getInputBuffer();
			switch (this->state) {
				case WebSocketState::Initializing:
					if (newVector.find("\r\n\r\n") != std::string::npos) {
						std::string headers = newVector.substr(0, newVector.find("\r\n\r\n"));
						newVector.erase(0, newVector.find("\r\n\r\n") + 4);
						std::vector<std::string> headerOut = tokenize(headers);
						if (headerOut.size()) {
							std::string statusLine = headerOut[0];
							headerOut.erase(headerOut.begin());
							std::vector<std::string> status = tokenize(statusLine, " ");
							this->state = WebSocketState::Connected;
							this->theClients[theIndex]->getInputBuffer().clear();
							this->theClients[theIndex]->getInputBuffer().insert(this->theClients[theIndex]->getInputBuffer().end(), newVector.begin(), newVector.end());
							this->parseHeader(theIndex);
						}
					}
					break;
				case WebSocketState::Connected:
					while (this->parseHeader(theIndex)) {
					};
					return;
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::handleBuffer()");
			}
			this->handleDroppedConnection(theIndex);
		}
	}

	bool BaseSocketAgent::parseHeader(SOCKET theIndex) noexcept {
		try {
			std::string newVector = this->theClients[theIndex]->getInputBuffer();
			if (this->theClients[theIndex]->getInputBuffer().size() < 4) {
				return false;
			} else {
				uint8_t theValue = this->theClients[theIndex]->getInputBuffer()[0];
				std::bitset<8> theBits = theValue;
				theBits.set(7, 0);
				this->opCode = static_cast<WebSocketOpCode>(theBits.to_ulong());
				switch (this->opCode) {
					case WebSocketOpCode::Op_Continuation:
					case WebSocketOpCode::Op_Text:
					case WebSocketOpCode::Op_Binary:
					case WebSocketOpCode::Op_Ping:
					case WebSocketOpCode::Op_Pong: {
						uint8_t length01 = this->theClients[theIndex]->getInputBuffer()[1];
						std::bitset<8> theBits02 = length01;
						theBits02.set(7, 0);
						uint32_t payloadStartOffset = 2;
						uint64_t length02 = theBits02.to_ullong();
						if (length02 == webSocketPayloadLengthMagicLarge) {
							if (this->theClients[theIndex]->getInputBuffer().size() < 8) {
								return false;
							}
							uint8_t length03 = this->theClients[theIndex]->getInputBuffer()[2];
							uint8_t length04 = this->theClients[theIndex]->getInputBuffer()[3];
							length02 = static_cast<uint64_t>((length03 << 8) | length04);
							payloadStartOffset += 2;
						} else if (length02 == webSocketPayloadLengthMagicHuge) {
							if (this->theClients[theIndex]->getInputBuffer().size() < 10) {
								return false;
							}
							length02 = 0;
							for (uint64_t value = 2, shift = 56; value < 10; ++value, shift -= 8) {
								uint8_t length05 = static_cast<uint8_t>(this->theClients[theIndex]->getInputBuffer()[value]);
								length02 |= static_cast<uint64_t>(length05) << shift;
							}
							payloadStartOffset += 8;
						}
						if (this->theClients[theIndex]->getInputBuffer().size() < payloadStartOffset + length02) {
							return false;
						} else {
							std::string newerVector{};
							newerVector.insert(newerVector.begin(), this->theClients[theIndex]->getInputBuffer().begin() + payloadStartOffset + 4,
								this->theClients[theIndex]->getInputBuffer().begin() + payloadStartOffset + length02 + 4);
							this->theClients[theIndex]->getInputBuffer() = std::move(newerVector);
							this->onMessageReceived(theIndex);
						}
						return true;
					}
					case WebSocketOpCode::Op_Close: {
						uint16_t close = this->theClients[theIndex]->getInputBuffer()[2] & 0xff;
						close <<= 8;
						close |= this->theClients[theIndex]->getInputBuffer()[3] & 0xff;
						this->closeCode = close;
						this->theClients[theIndex]->getInputBuffer().clear();
						this->respondToDisconnect(theIndex);
						return false;
					}
					default: {
						this->closeCode = 0;
						return false;
					}
				}
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::parseHeader()");
			}
			this->handleDroppedConnection(theIndex);
			return false;
		}
	}

	void BaseSocketAgent::connectInternal() noexcept {
		try {
			if (this->discordCoreClient->configParser.getTheData().doWePrintGeneralErrorMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				if (this->currentConnectionData[0] == 0) {
					std::cout << shiftToBrightBlue() << "Connecting Shard 1 of unknown number...." << reset() << std::endl;
				} else {
					std::cout << shiftToBrightBlue() << "Connecting Shard " + std::to_string(this->currentConnectionData[0].get<int32_t>() + 1) << " of "
							  << this->currentConnectionData[1].get<int32_t>()
							  << std::string(" Shards for this process. (") + std::to_string(this->currentConnectionData[0].get<int32_t>() + 1) + " of " +
							std::to_string(this->currentConnectionData[1].get<int32_t>()) + std::string(" Shards total across all processes)")
							  << reset() << std::endl;
				}
			}
			auto theClient = this->discordCoreClient->webSocketSSLServerMain->connectShard(this->currentNewSocket, this->currentConnectionData[0].get<int32_t>(),
				this->currentConnectionData[1].get<int32_t>());
			SOCKET theSocket = theClient->clientSocket;
			this->currentIndex = theSocket;

			this->theClients.insert_or_assign(theSocket, std::move(theClient));
			this->state = WebSocketState::Initializing;

			while (this->authKey == "") {
				auto returnValue = this->webSocketSSLServerMain->processIO(this->theClients);
				if (returnValue.returnCode == ProcessIOReturnCode::Error) {
					theSocket = SOCKET_ERROR;
				} else {
					this->handleBuffer(theSocket);
				}
			}

			std::string sendString = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ";
			sendString += this->authKey + "\r\n\r\n";
			this->sendMessage(&sendString, theSocket);
			auto returnValue = this->webSocketSSLServerMain->processIO(this->theClients);
			if (returnValue.returnCode == ProcessIOReturnCode::Error) {
				theSocket = SOCKET_ERROR;
			} else {
				this->handleBuffer(theSocket);
			}
			this->sendHelloMessage(theSocket);
			this->sendFinalMessage(theSocket);
			while (static_cast<int32_t>(returnValue.returnIndex) == 0) {
				returnValue = this->webSocketSSLServerMain->processIO(this->theClients);
				this->handleBuffer(theSocket);
			}
			this->doWeQuit->store(false);
			this->doWeConnect.store(false);
			if (this->discordCoreClient->configParser.getTheData().doWePrintGeneralErrorMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				if (this->currentConnectionData[0] == 0) {
					std::cout << shiftToBrightGreen() << "Connected Shard 1 of unknown number." << reset() << std::endl;
				} else {
					std::cout << shiftToBrightGreen() << "Connected Shard " + std::to_string(this->currentConnectionData[0].get<int32_t>() + 1) << " of "
							  << this->currentConnectionData[1].get<int32_t>()
							  << std::string(" Shards for this process. (") + std::to_string(this->currentConnectionData[0].get<int32_t>() + 1) + " of " +
							std::to_string(this->currentConnectionData[1].get<int32_t>()) + std::string(" Shards total across all processes)")
							  << reset() << std::endl;
				}
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::connect()");
			}
		}
	}

	void BaseSocketAgent::connect(int32_t currentShard, int32_t totalShardCount) noexcept {
		this->currentConnectionData.clear();
		this->currentConnectionData.push_back(currentShard);
		this->currentConnectionData.push_back(totalShardCount);
		this->currentNewSocket = this->webSocketSSLServerMain->getNewSocket();
		this->doWeConnect.store(true);
		while (this->doWeConnect.load()) {
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
		}
	}

	BaseSocketAgent::~BaseSocketAgent() noexcept {
		this->theTask->request_stop();
		if (this->theTask->joinable()) {
			this->theTask->join();
		}
	}
};