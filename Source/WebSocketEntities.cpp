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
		this->doWeQuit = doWeQuitNew;
		this->theTask = std::make_unique<std::jthread>([this](std::stop_token theToken) {
			this->run(theToken);
		});
	}

	void BaseSocketAgent::sendMessage(std::string* dataToSend, WebSocketSSLShard& theIndex) noexcept {
		try {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessSentMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				std::cout << shiftToBrightBlue() << "Sending WebSocket " + theIndex.shard.dump() + std::string("'s Message: ") << std::endl << *dataToSend << reset();
			}

			if (this->webSocketSSLServerMain != nullptr) {
				theIndex.writeData(*dataToSend);
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::sendMessage()");
			}
		}
	}

	std::jthread* BaseSocketAgent::getTheTask() noexcept {
		return this->theTask.get();
	}

	void BaseSocketAgent::sendMessage(nlohmann::json& dataToSend, WebSocketSSLShard& theIndex) noexcept {
		try {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessSentMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				std::cout << shiftToBrightBlue() << "Sending WebSocket " + theIndex.shard.dump() + std::string("'s Message: ") << dataToSend.dump() << reset() << reset()
						  << std::endl
						  << std::endl;
			}
			std::string theString{};
			this->stringifyJsonData(dataToSend, theString);
			theIndex.writeData(theString);

		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::sendMessage()");
			}
			this->respondToDisconnect(theIndex);
		}
	}

	uint64_t BaseSocketAgent::createHeader(std::string& outBuffer, uint64_t sendLength, WebSocketOpCode opCodeNew) noexcept {
		try {
			outBuffer.push_back(static_cast<uint8_t>(opCodeNew) | webSocketFinishBit);

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
			return uint64_t{};
		}
	}

	void BaseSocketAgent::sendCreateGuild(WebSocketSSLShard& theIndex) noexcept {
		if (theIndex.currentGuildCount < theIndex.totalGuildCount) {
			theIndex.currentGuildCount += 1;
			theIndex.lastNumberSent += 1;
			auto theGuildNew = this->discordCoreClient->theGuildHolder;
			theGuildNew["d"]["id"] = this->jsonifier.randomizeId();
			theGuildNew["s"] = theIndex.lastNumberSent;
			WebSocketMessage theMessage{};
			theMessage.jsonMsg = std::move(theGuildNew);
			if (this->theMode == WebSocketMode::ETF) {
				theMessage.theOpCode = WebSocketOpCode::Op_Binary;
			} else {
				theMessage.theOpCode = WebSocketOpCode::Op_Text;
			}
			theIndex.theMessageQueue.push(std::move(theMessage));
		}
	}

	void BaseSocketAgent::sendReadyMessage(WebSocketSSLShard& theIndex) noexcept {
		theIndex.lastNumberSent += 1;
		nlohmann::json jsonData{};
		jsonData["op"] = static_cast<int8_t>(0);
		jsonData["s"] = theIndex.lastNumberSent;
		jsonData["t"] = "READY";
		jsonData["d"]["session_id"] = this->jsonifier.randomizeId();
		jsonData["d"]["guilds"];
		int32_t guildSize = static_cast<int32_t>(
			floor(static_cast<double>(this->discordCoreClient->guildQuantity) / static_cast<double>(this->discordCoreClient->shardingOptions.totalNumberOfShards)));
		theIndex.totalGuildCount = guildSize;
		if (guildSize > 2500) {
			this->initDisconnect(WebSocketCloseCode::Sharding_Required, theIndex);
			return;
		}
		for (int32_t x = 0; x < this->discordCoreClient->guildQuantity / this->discordCoreClient->shardingOptions.totalNumberOfShards; x += 1) {
			auto theDifference = UINT64_MAX / this->discordCoreClient->shardingOptions.totalNumberOfShards;
			auto minValue = static_cast<uint64_t>(theDifference) * static_cast<uint64_t>(theIndex.shard[0]);
			auto maxValue = minValue + theDifference;
			auto theGuild = this->jsonifier.generateUnavailableGuild(minValue, maxValue);
			jsonData["d"]["guilds"].push_back(this->jsonifier.JSONIFYUnavailableGuild(theGuild));
			this->theGuilds[theIndex.clientSocket].push_back(theGuild);
		}
		jsonData["d"]["v"] = 10;
		jsonData["d"]["user"] = this->jsonifier.JSONIFYUser(std::move(*this->jsonifier.generateUser()));
		std::string theString{};
		this->stringifyJsonData(jsonData, theString);
		theIndex.writeData(theString);
	}

	void BaseSocketAgent::sendHelloMessage(WebSocketSSLShard& theIndex) noexcept {
		nlohmann::json jsonData{};
		jsonData["op"] = static_cast<int8_t>(10);
		jsonData["s"];
		jsonData["t"];
		jsonData["d"]["heartbeat_interval"] = static_cast<uint16_t>(this->heartbeatInterval);
		jsonData["d"]["_trace"] = nlohmann::json::array();
		jsonData["d"]["_trace"].push_back(std::string({ "[\"gateway-prd-main-6dtt\",{\"micros\":0.0}]" }));
		std::string theString{};
		this->stringifyJsonData(jsonData, theString);
		theIndex.writeData(theString);
	}

	void BaseSocketAgent::sendHeartBeat(WebSocketSSLShard& theIndex) noexcept {
		nlohmann::json jsonData{};
		jsonData["op"] = static_cast<int8_t>(11);
		jsonData["s"];
		jsonData["t"];
		jsonData["d"];
		std::string theString{};
		this->stringifyJsonData(jsonData, theString);
		theIndex.writeData(theString);
	}

	std::vector<std::string> BaseSocketAgent::tokenize(const std::string& dataIn, WebSocketSSLShard& theIndex, const std::string& separator) noexcept {
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
					this->authKeys[theIndex.clientSocket] =
						dataOut[dataOut.size() - 1].substr(dataOut[dataOut.size() - 1].find("Sec-WebSocket-Key: ") + std::string{ "Sec-WebSocket-Key: " }.size());
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
			this->respondToDisconnect(theIndex);
			return std::vector<std::string>{};
		}
	}

	void BaseSocketAgent::respondToDisconnect(WebSocketSSLShard& theIndex) noexcept {
		if (theIndex.clientSocket != 0) {
			std::string theString{};
			theString.push_back(static_cast<int8_t>(WebSocketOpCode::Op_Close) | static_cast<int8_t>(webSocketFinishBit));
			theString.push_back(0);
			theString.push_back(static_cast<int8_t>(static_cast<uint16_t>(1000) >> 8));
			theString.push_back(static_cast<int8_t>(1000 & 0xff));
			if (this->webSocketSSLServerMain != nullptr) {
				if (this->theClients.contains(theIndex.clientSocket)) {
					theIndex.writeData(theString);
					if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
						std::cout << shiftToBrightRed() << "WebSocket " + theIndex.shard.dump() + " Closed; Code: " << this->closeCode << reset() << std::endl;
					}
				}
			}
			if (this->theClients.contains(theIndex.clientSocket)) {
				ReconnectionPackage theData{};
				theData.currentShard = theIndex.shard[0];
				theData.totalShardCount = theIndex.shard[1];
				theData.theMap = &this->theClients;
				theData.theSocket = theIndex.clientSocket;
				this->theClients.erase(theIndex.clientSocket);
				this->webSocketSSLServerMain->submitReconnectionShard(theData);
			} else {
				this->doWeQuit->store(true);
				this->theTask->request_stop();
			}
		}
	}

	void BaseSocketAgent::initDisconnect(WebSocketCloseCode reason, WebSocketSSLShard& theIndex) noexcept {
		if (theIndex.clientSocket != 0) {
			std::string theString{};
			theString.push_back(static_cast<int8_t>(WebSocketOpCode::Op_Close) | static_cast<int8_t>(webSocketFinishBit));
			theString.push_back(0);
			theString.push_back(static_cast<uint16_t>(reason) >> 8);
			theString.push_back(static_cast<int8_t>(reason) & 0xff);
			if (this->theClients.contains(theIndex.clientSocket)) {
				while (theIndex.theMessageQueue.size() > 0) {
					theIndex.theMessageQueue.pop();
				}
				WebSocketMessage theMessage{};
				theMessage.stringMsg = theString;
				theMessage.theOpCode = WebSocketOpCode::Op_Close;
				theIndex.writeData(theString);
			}
		}
	}

	void BaseSocketAgent::sendFinalMessage(WebSocketSSLShard& theIndex) noexcept {
		if (theIndex.outputBuffer.size() == 0) {
			if (theIndex.theMessageQueue.size() > 0) {
				WebSocketMessage newMessage = std::move(theIndex.theMessageQueue.front());
				theIndex.theMessageQueue.pop();
				if (newMessage.stringMsg.size() > 0) {
					this->sendMessage(&newMessage.stringMsg, theIndex);
				} else {
					if (this->theMode == WebSocketMode::ETF) {
						this->sendMessage(newMessage.jsonMsg, theIndex);
					} else {
						this->sendMessage(newMessage.jsonMsg, theIndex);
					}
				}
			}
		}
	}

	void BaseSocketAgent::run(std::stop_token theToken) noexcept {
		try {
			StopWatch theStopWatch{ 3500ms };
			theStopWatch.resetTimer();
			while (!theToken.stop_requested() && !this->doWeQuit->load()) {
				if (this->doWeConnect.load()) {
					this->connectInternal();
				}
				if (this->theClients.size() > 0) {
					for (auto& [key, value]: this->theClients) {
						this->webSocketSSLServerMain->processIO(this->theClients);
						if (this->doWeConnect.load()) {
							this->connectInternal();
						}
						this->parseHeader(*value);
						if (this->closeCode == 0) {
							if (value->outputBuffer.size() == 0 && theStopWatch.hasTimePassed()) {
								theStopWatch.resetTimer();
								if (value->sendGuilds) {
									this->sendCreateGuild(*value);
								}
								this->sendFinalMessage(*value);
							}
						} else {
							this->closeCode = 0;
							break;
						}
					}
					if (this->doWeConnect.load()) {
						this->connectInternal();
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::run()");
			}
		}
	}

	void BaseSocketAgent::sendGuildMemberChunks() noexcept {
	}

	void BaseSocketAgent::onMessageReceived(WebSocketSSLShard& theIndex) noexcept {
		try {
			std::string messageNew{};

			messageNew = std::move(theIndex.getInputBuffer());
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

			if (payload["op"] == 8) {
				this->sendGuildMemberChunks();
				theIndex.getInputBuffer().clear();
			}

			if (payload["op"] == 1) {
				this->sendHeartBeat(theIndex);
				theIndex.getInputBuffer().clear();
			}

			if (payload["op"] == 2) {
				if (payload["d"].contains("shard")) {
					theIndex.shard.clear();
					theIndex.shard = payload["d"]["shard"];
					this->intentsValue = payload["d"]["intents"];
				}
				if (!this->discordCoreClient->haveWeCollectedShardingInfo) {
					this->discordCoreClient->shardingOptions.totalNumberOfShards = theIndex.shard[1];
					this->discordCoreClient->haveWeCollectedShardingInfo = true;
					this->discordCoreClient->shardingOptions.startingShard = theIndex.shard[0];
				}
				if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessReceiveMessages) {
					std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
					std::cout << shiftToBrightGreen() << "Message received from WebSocket " + theIndex.shard.dump() + ": " << payload.dump() << reset() << std::endl << std::endl;
				}
				this->sendReadyMessage(theIndex);
				theIndex.getInputBuffer().clear();
				theIndex.sendGuilds = true;
				if (this->discordCoreClient->configParser.getTheData().doWePrintGeneralSuccessMessages) {
					std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
					std::cout << shiftToBrightGreen() << "Connected Shard " + std::to_string(theIndex.shard[0].get<int32_t>() + 1) << " of " << theIndex.shard[1].get<int32_t>()
							  << std::string(" Shards for this process. (") + std::to_string(theIndex.shard[0].get<int32_t>() + 1) + " of " +
							std::to_string(theIndex.shard[1].get<int32_t>()) + std::string(" Shards total across all processes)")
							  << reset() << std::endl;
				}

				if (this->discordCoreClient->configParser.getTheData().doWePrintGeneralSuccessMessages) {
					if (theIndex.shard[0] < theIndex.shard[1] - 1) {
						std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
						std::cout << shiftToBrightBlue() << "Connecting the next Shard..." << reset() << std::endl << std::endl;
					}
				}
				theIndex.getInputBuffer().clear();
			} else {
				if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessReceiveMessages) {
					std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
					std::cout << shiftToBrightGreen() << "Message received from WebSocket " + theIndex.shard.dump() + ": " << payload.dump() << reset() << std::endl << std::endl;
				}
			}

			return;
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::onMessageReceived()");
			}
			this->respondToDisconnect(theIndex);
			return;
		}
	}

	bool BaseSocketAgent::parseHeader(WebSocketSSLShard& theIndex) noexcept {
		std::string newVector = theIndex.getInputBuffer();
		switch (theIndex.theState) {
			case WebSocketState::Initializing:
				if (newVector.find("\r\n\r\n") != std::string::npos) {
					std::string headers = newVector.substr(0, newVector.find("\r\n\r\n"));
					newVector.erase(0, newVector.find("\r\n\r\n") + 4);
					std::vector<std::string> headerOut = tokenize(headers, theIndex);
					if (headerOut.size()) {
						std::string statusLine = headerOut[0];
						headerOut.erase(headerOut.begin());
						std::vector<std::string> status = tokenize(statusLine, theIndex, " ");
						theIndex.theState = WebSocketState::Connected;
						theIndex.getInputBuffer().clear();
						theIndex.getInputBuffer().insert(theIndex.getInputBuffer().end(), newVector.begin(), newVector.end());
					}
				}
				break;
			case WebSocketState::Connected:
				if (theIndex.getInputBuffer().size() < 4) {
					return false;
				} else {
					uint8_t theValue = theIndex.getInputBuffer()[0];
					std::bitset<8> theBits = theValue;
					theBits.set(7, 0);
					this->opCode = static_cast<WebSocketOpCode>(theBits.to_ulong());
					switch (this->opCode) {
						case WebSocketOpCode::Op_Continuation:
						case WebSocketOpCode::Op_Text:
						case WebSocketOpCode::Op_Binary:
						case WebSocketOpCode::Op_Ping:
						case WebSocketOpCode::Op_Pong: {
							uint8_t length01 = theIndex.getInputBuffer()[1];
							std::bitset<8> theBits02 = length01;
							theBits02.set(7, 0);
							uint32_t payloadStartOffset = 2;
							uint64_t length02 = theBits02.to_ullong();
							if (length02 == webSocketPayloadLengthMagicLarge) {
								if (theIndex.getInputBuffer().size() < 8) {
									return false;
								}
								uint8_t length03 = theIndex.getInputBuffer()[2];
								uint8_t length04 = theIndex.getInputBuffer()[3];
								length02 = static_cast<uint64_t>((length03 << 8) | length04);
								payloadStartOffset += 2;
							} else if (length02 == webSocketPayloadLengthMagicHuge) {
								if (theIndex.getInputBuffer().size() < 10) {
									return false;
								}
								length02 = 0;
								for (uint64_t value = 2, shift = 56; value < 10; ++value, shift -= 8) {
									uint8_t length05 = static_cast<uint8_t>(theIndex.getInputBuffer()[value]);
									length02 |= static_cast<uint64_t>(length05) << shift;
								}
								payloadStartOffset += 8;
							}
							if (theIndex.getInputBuffer().size() < payloadStartOffset + length02) {
								return false;
							} else {
								std::string newerVector{};
								newerVector.insert(newerVector.begin(), theIndex.getInputBuffer().begin() + payloadStartOffset + 4,
									theIndex.getInputBuffer().begin() + payloadStartOffset + length02 + 4);
								theIndex.getInputBuffer() = std::move(newerVector);
								this->onMessageReceived(theIndex);
							}
							return true;
						}
						case WebSocketOpCode::Op_Close: {
							uint16_t close = theIndex.getInputBuffer()[2] & 0xff;
							close <<= 8;
							close |= theIndex.getInputBuffer()[3] & 0xff;
							this->closeCode = close;
							theIndex.getInputBuffer().clear();
							this->respondToDisconnect(theIndex);
							return false;
						}
						default: {
							this->closeCode = 1000;
							return false;
						}
					}
				}
				return true;
		}
	}

	void BaseSocketAgent::stringifyJsonData(const nlohmann::json& jsonData, std::string& theString) noexcept {
		std::string theVector{};
		WebSocketOpCode theOpCode{};
		if (this->theMode == DiscordCoreLoader::WebSocketMode::ETF) {
			theVector = this->erlPacker.parseJsonToEtf(jsonData);
			theOpCode = WebSocketOpCode::Op_Binary;
		} else {
			theVector = jsonData.dump();
			theOpCode = WebSocketOpCode::Op_Text;
		}
		std::string header{};
		this->createHeader(header, theVector.size(), theOpCode);
		theString.insert(theString.begin(), header.begin(), header.end());
		theString.insert(theString.begin() + header.size(), theVector.begin(), theVector.end());
	}

	void BaseSocketAgent::connectInternal() noexcept {
		try {
			auto theClient = this->discordCoreClient->webSocketSSLServerMain->connectShard(this->currentNewSocket);
			SOCKET theSocket = theClient->clientSocket;
			this->currentIndex = theSocket;
			std::unordered_map<SOCKET, std::unique_ptr<WebSocketSSLShard>> theMap{};
			theMap[theSocket] = std::move(theClient);
			theMap[theSocket]->theState = WebSocketState::Initializing;
			theMap[theSocket]->shard.push_back(theSocket);
			while (this->authKeys[theSocket] == "") {
				this->webSocketSSLServerMain->processIO(theMap);
				this->parseHeader(*theMap[theSocket]);
			}

			std::string sendString = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ";
			sendString += this->authKeys[theSocket] + "\r\n\r\n";
			this->sendMessage(&sendString, *theMap[theSocket]);
			this->webSocketSSLServerMain->processIO(theMap);
			this->parseHeader(*theMap[theSocket]);
			this->sendHelloMessage(*theMap[theSocket]);
			this->sendFinalMessage(*theMap[theSocket]);
			this->webSocketSSLServerMain->processIO(theMap);
			this->parseHeader(*theMap[theSocket]);
			this->theClients[theSocket] = std::move(theMap[theSocket]);
			this->doWeQuit->store(false);
			this->doWeConnect.store(false);
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::connect()");
			}
		}
	}

	void BaseSocketAgent::connect() noexcept {
		this->currentNewSocket = this->webSocketSSLServerMain->getNewSocket();
		this->doWeConnect.store(true);
	}

	BaseSocketAgent::~BaseSocketAgent() noexcept {
		this->theTask->request_stop();
		if (this->theTask->joinable()) {
			this->theTask->join();
		}
	}
};