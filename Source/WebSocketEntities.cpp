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
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file WebSocketEntities.cpp

#include <discordcoreloader/WebSocketEntities.hpp>
#include <discordcoreloader/DiscordCoreClient.hpp>
#include <discordcoreloader/JsonSpecializations.hpp>

namespace DiscordCoreLoader {

	constexpr uint16_t webSocketMaxPayloadLengthLarge{ 65535 };
	constexpr uint8_t webSocketPayloadLengthMagicLarge{ 126 };
	constexpr uint8_t webSocketPayloadLengthMagicHuge{ 127 };
	constexpr uint8_t maxHeaderSize{ sizeof(uint64_t) + 2 };
	constexpr uint8_t webSocketMaxPayloadLengthSmall{ 125 };
	constexpr uint8_t webSocketFinishBit{ (1u << 7u) };
	constexpr uint8_t webSocketMaskBit{ (1u << 7u) };

	WebSocketSSLShard::WebSocketSSLShard(SOCKET theSocket, SSL_CTX* theContext, bool doWePrintErrorsNew, BaseSocketAgent* theAgentNew)
		: SSLClient(theSocket, theContext, doWePrintErrorsNew) {
		this->theAgent = theAgentNew;
	}

	void WebSocketSSLShard::handleBuffer() noexcept {
		theAgent->handleBuffer(this);
	}

	BaseSocketAgent::BaseSocketAgent(WebSocketSSLServerMain* webSocketSSLServerMainNew, DiscordCoreClient* discordCoreClient,
		std::atomic_bool* doWeQuitNew, bool doWeInstantiateAThread) noexcept {
		this->webSocketSSLServerMain = webSocketSSLServerMainNew;
		this->discordCoreClient = discordCoreClient;
		this->doWeQuit = doWeQuitNew;
		this->randomizer = discordCoreClient->configParser.getTheData();
		if (doWeInstantiateAThread) {
			this->theTask = std::make_unique<std::jthread>([this](std::stop_token theToken) {
				this->run(theToken);
			});
		}
	}

	void BaseSocketAgent::sendMessage(std::string* dataToSend, SSLClient* theShard, bool priority) noexcept {
		try {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessSentMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				std::cout << shiftToBrightBlue() << "Sending WebSocket " + std::to_string(theShard->shard[0]) + std::string("'s Message: ")
						  << std::endl
						  << *dataToSend << reset();
			}

			if (this->webSocketSSLServerMain != nullptr) {
				theShard->writeData(*dataToSend, priority);
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::sendMessage()");
			}
			theShard->disconnect();
		}
	}

	std::jthread* BaseSocketAgent::getTheTask() noexcept {
		return this->theTask.get();
	}

	void BaseSocketAgent::sendMessage(std::string&& dataToSend, WebSocketOpCode theOpCode, SSLClient* theShard, bool priority) noexcept {
		try {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessSentMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				std::cout << shiftToBrightBlue() << "Sending WebSocket " + std::to_string(theShard->shard[0]) + std::string("'s Message: ")
						  << std::string{ dataToSend } << reset() << reset() << std::endl
						  << std::endl;
			}
			std::string header{};
			this->createHeader(header, dataToSend.size(), theOpCode);
			std::string theVectorNew{};
			theVectorNew.insert(theVectorNew.begin(), header.begin(), header.end());
			theVectorNew.insert(theVectorNew.end(), dataToSend.begin(), dataToSend.end());
			theShard->writeData(theVectorNew, priority);
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::sendMessage()");
			}
			theShard->disconnect();
		}
	}

	void BaseSocketAgent::createHeader(std::string& outBuffer, uint64_t sendLength, WebSocketOpCode opCodeNew) noexcept {
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
			return;
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::createHeader()");
			}
			return;
		}
	}

	void BaseSocketAgent::sendResumedPayload(SSLClient* theShard) noexcept {
		theShard->lastNumberSent++;
		WebSocketMessageReal<int32_t> data{};
		data.t = "RESUMED";
		data.op = static_cast<int8_t>(0);
		data.s = theShard->lastNumberSent;
		parser.serializeJson(data, stringBuffer);
		if (theShard->theMode == WebSocketMode::JSON) {
			theShard->theOpCode = WebSocketOpCode::Op_Text;
		} else {
			theShard->theOpCode = WebSocketOpCode::Op_Binary;
		}
		this->sendMessage(std::move(stringBuffer), theShard->theOpCode, theShard, true);
	}

	void BaseSocketAgent::generateGuildData() noexcept {
		for (auto& [key, value]: this->theClients) {
			std::string theString{};
			if (!value->doWeHaveOurGuild) {
				this->randomizer.randomizeId(theString);
				value->theGuildHolder = this->randomizer.generateGuild(theString);
				value->doWeHaveOurGuild = true;
			}
		}
	}

	void BaseSocketAgent::sendCreateGuilds(SSLClient* theShard) noexcept {
		if (theShard->currentGuildCount < theShard->totalGuildCount && theShard->doWeHaveOurGuild) {
			theShard->currentGuildCount++;
			theShard->lastNumberSent++;
			for (auto& value: theShard->theGuildHolder.roles) {
				std::string theString{};
				this->randomizer.randomizeId(theString);
				value.id = std::move(theString);
			}
			for (auto& value: theShard->theGuildHolder.members) {
				std::string theString{};
				this->randomizer.randomizeId(theString);
				value.id = std::move(theString);
			}
			for (auto& value: theShard->theGuildHolder.channels) {
				std::string theString{};
				this->randomizer.randomizeId(theString);
				value.id = std::move(theString);
			}
			std::string theString{};
			this->randomizer.randomizeId(theString);
			WebSocketMessageReal<GuildData> data{};
			data.d = theShard->theGuildHolder;
			data.d.id = theString;
			data.s = theShard->lastNumberSent;
			data.t = "GUILD_CREATE";
			if (theShard->theMode == WebSocketMode::JSON) {
				theShard->theOpCode = WebSocketOpCode::Op_Text;
			} else {
				theShard->theOpCode = WebSocketOpCode::Op_Binary;
			}
			parser.serializeJson(data, stringBuffer);
			this->sendMessage(std::move(stringBuffer), theShard->theOpCode, theShard, true);
		}
	}
	
	void BaseSocketAgent::sendReadyMessage(SSLClient* theShard) noexcept {
		theShard->lastNumberSent++;
		WebSocketMessageReal<ReadyData> data{};
		data.s = theShard->lastNumberSent;
		data.t = "READY";
		data.d.resumeGateWayUrl = "192.168.0.15";
		std::string theString{};
		this->randomizer.randomizeId(theString);
		data.d.v = 10;
		data.d.sessionId = std::move(theString);
		data.d.user = this->randomizer.generateUser();
		int32_t guildSize = this->discordCoreClient->guildQuantity.load() / this->discordCoreClient->totalShardCount.load();
		theShard->totalGuildCount = guildSize;
		if (guildSize > 2500) {
			this->initDisconnect(WebSocketCloseCode::Sharding_Required, theShard);
			return;
		}
		if (theShard->theMode == WebSocketMode::JSON) {
			theShard->theOpCode = WebSocketOpCode::Op_Text;
		} else {
			theShard->theOpCode = WebSocketOpCode::Op_Binary;
		}
		parser.serializeJson(data, stringBuffer);
		this->sendMessage(std::move(stringBuffer), theShard->theOpCode, theShard, true);
	}

	void BaseSocketAgent::sendHelloMessage(SSLClient* theShard) noexcept {
		WebSocketMessageReal<HelloData> data{};
		data.op = 10;
		data.d.heartBeatInterval = static_cast<uint16_t>(this->heartbeatInterval);
		if (theShard->theMode == WebSocketMode::JSON) {
			theShard->theOpCode = WebSocketOpCode::Op_Text;
		} else {
			theShard->theOpCode = WebSocketOpCode::Op_Binary;
		}
		parser.serializeJson(data, stringBuffer);
		this->sendMessage(std::move(stringBuffer), theShard->theOpCode, theShard, true);
	}

	void BaseSocketAgent::sendHeartBeat(SSLClient* theShard) noexcept {
		WebSocketMessageReal<int32_t> data{};
		data.op = 11;
		data.d = theShard->lastNumberSent;
		if (theShard->theMode == WebSocketMode::JSON) {
			theShard->theOpCode = WebSocketOpCode::Op_Text;
		} else {
			theShard->theOpCode = WebSocketOpCode::Op_Binary;
		}
		parser.serializeJson(data, stringBuffer);
		this->sendMessage(std::move(stringBuffer), theShard->theOpCode, theShard, true);
	}

	std::vector<std::string> BaseSocketAgent::tokenize(const std::string& dataIn, SSLClient* theShard, const std::string& separator) noexcept {
		try {
			size_t value{ 0 };
			std::vector<std::string> dataOut{};
			while ((value = dataIn.find_first_not_of(separator, value)) != std::string::npos) {
				auto output = dataIn.find(separator, value);
				dataOut.emplace_back(dataIn.substr(value, output - value));
				if (dataIn.substr(value, output - value).find("GET") != std::string::npos) {
					std::string newString = dataIn.substr(dataIn.find("v=") + 2, dataIn.length() - dataIn.find("v=") + 2);
					int8_t index{ 0 };
					for (auto& value02: newString) {
						index++;
						if (value != isdigit(value02)) {
							index++;
							break;
						}
					}
					int32_t version = std::stoi(newString.substr(0, index));
					if (version != 10) {
						this->initDisconnect(WebSocketCloseCode::Invalid_API_Version, theShard);
					}
				}
				if (dataIn.substr(value, output - value).find("Sec-WebSocket-Key:") != std::string::npos) {
					theShard->authKey = dataOut[dataOut.size() - 1].substr(
						dataOut[dataOut.size() - 1].find("Sec-WebSocket-Key: ") + std::string{ "Sec-WebSocket-Key: " }.size());
				}
				if (dataIn.substr(value, output - value).find("json") != std::string::npos) {
					theShard->theMode = WebSocketMode::JSON;
					theShard->theOpCode = WebSocketOpCode::Op_Text;
				} else if (dataIn.substr(value, output - value).find("etf") != std::string::npos) {
					theShard->theMode = WebSocketMode::ETF;
					theShard->theOpCode = WebSocketOpCode::Op_Binary;
				}
				value = output;
			}
			return dataOut;
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::tokenize()");
			}
			theShard->disconnect();
			return std::vector<std::string>{};
		}
	}

	void BaseSocketAgent::initDisconnect(WebSocketCloseCode reason, SSLClient* theShard) noexcept {
		if (theShard != 0) {
			std::string theString{};
			theString.push_back(static_cast<int8_t>(WebSocketOpCode::Op_Close) | static_cast<int8_t>(webSocketFinishBit));
			theString.push_back(0);
			theString.push_back(static_cast<uint16_t>(reason) >> 8);
			theString.push_back(static_cast<int8_t>(reason) & 0xff);
			while (theShard->theMessageQueue.size() > 0) {
				theShard->theMessageQueue.pop_front();
			}
			WebSocketMessage theMessage{};
			theMessage.d = theString;
			theMessage.theOpCode = WebSocketOpCode::Op_Close;
			theShard->theMessageQueue.push_front(theMessage);
		}
	}

	void BaseSocketAgent::sendFinalMessage(SSLClient* theShard) noexcept {
		if (theShard->outputBuffers.size() == 0) {
			if (theShard->theMessageQueue.size() > 0) {
				WebSocketMessage newMessage = std::move(theShard->theMessageQueue.front());
				theShard->theMessageQueue.pop_front();
				this->sendMessage(std::move(newMessage.d), newMessage.theOpCode, theShard, false);
			}
		}
	}

	void BaseSocketAgent::run(std::stop_token theToken) noexcept {
		try {
			while (!theToken.stop_requested() && !this->doWeQuit->load()) {
				if (this->theClients.size() > 0) {
					std::vector<WebSocketSSLShard*> theVector{};
					for (auto& [key, value]: this->theClients) {
						if (value && value->areWeStillConnected() && value->areWeConnected) {
							theVector.emplace_back(value.get());
						}
					}
					if (theVector.size() == 0) {
						std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
					}
					theVector = this->webSocketSSLServerMain->processIO(theVector);
					for (auto& value: theVector) {
						if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
							std::unique_lock theLockTwo{ this->discordCoreClient->coutMutex };
							std::cout << shiftToBrightRed() << "Connection lost for WebSocket [" << std::to_string(value->shard[0]) << ","
									  << this->discordCoreClient->totalShardCount.load() << "]... reconnecting." << reset() << std::endl
									  << std::endl;
						}
						value->disconnect();
					}
					if (this->theClients.size() > this->currentClientSize) {
						this->generateGuildData();
						this->currentClientSize = static_cast<int32_t>(this->theClients.size());
					}
					for (auto& [key, value]: this->theClients) {
						if (value && key != -1) {
							if (this->closeCode == 0) {
								if (value->outputBuffers.size() == 0) {
									if (value->sendGuilds) {
										this->sendCreateGuilds(value.get());
									}
									this->sendFinalMessage(value.get());
								}
							} else {
								this->closeCode = 0;
								break;
							}
						}
					}
				}
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::run()");
			}
		}
	}

	void BaseSocketAgent::sendGuildMemberChunks(SSLClient*) noexcept {
	}

	void BaseSocketAgent::onMessageReceived(WebSocketSSLShard* theShard, std::string& theString) noexcept {
		try {
			std::string refString{};
			std::string& payload{ refString };
			bool returnValue{ false };

			WebSocketMessage theMessage{};
			if (theString.size() > 0) {
				returnValue = true;

				if (theShard->theOpCode == WebSocketOpCode::Op_Binary) {
					try {
						payload = EtfParser::parseEtfToJson(theString);
						parser.parseJson(theMessage, payload);

					} catch (...) {
						returnValue = false;
					}
				} else {
					payload = theString;
					parser.parseJson(theMessage, payload);
				}
			}
			if (true) {
				if (theMessage.s != 0) {
					theShard->lastNumberSent = theMessage.s;
				}
				switch (theMessage.op) {
					case 1: {
						this->sendHeartBeat(theShard);
						theShard->getInputBuffer().clear();
						break;
					}
					case 2: {
						WebSocketMessageReal<WebSocketIdentifyData> identifyData{};
						parser.parseJson(identifyData, payload);
						if (!this->discordCoreClient->haveWeCollectedShardingInfo) {
							this->discordCoreClient->haveWeCollectedShardingInfo = true;
							this->discordCoreClient->shardingOptions.startingShard = identifyData.d.shard[0];
							this->discordCoreClient->totalShardCount.store(identifyData.d.shard[1]);
						}
						auto theCount = std::thread::hardware_concurrency() > this->discordCoreClient->totalShardCount.load()
							? this->discordCoreClient->totalShardCount.load()
							: std::thread::hardware_concurrency();
						if (theCount < 1) {
							theCount = 1;
						}
						this->discordCoreClient->workerCount.store(theCount);
						theShard->areWeConnected = true;
						this->discordCoreClient->currentShardIndex.store(identifyData.d.shard[0]);
						auto intents = identifyData.d.intents;
						this->intentsValue = static_cast<GatewayIntents>(intents);
						theShard->currentSocketIndex = this->discordCoreClient->currentShardIndex.load();
						theShard->shard[0] = identifyData.d.shard[0];
						theShard->shard[1] = identifyData.d.shard[1];
						this->sendReadyMessage(theShard);
						break;
					}
					case 8: {
						this->sendGuildMemberChunks(theShard);
						break;
					}
					case 6: {
						this->sendGuildMemberChunks(theShard);
						break;
					}
					default: {
						break;
					}
				}
				std::cout << shiftToBrightGreen()
						  << "Message received from WebSocket [" + std::to_string(theShard->shard[0]) + "," + std::to_string(theShard->shard[1]) +
						"]" + std::string(": ")
						  << payload << std::endl;
				if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessReceiveMessages) {
					
				}
			}
		} catch (...) {
			reportException("BaseSocektAgent::onMessageReceived()");
			return;
		}
	}

	void BaseSocketAgent::handleBuffer(SSLClient* theShard) noexcept {
		try {
			std::string newVector = theShard->getInputBuffer();
			switch (theShard->theState) {
				case WebSocketState::Initializing:
					if (newVector.find("\r\n\r\n") != std::string::npos) {
						std::string headers = newVector.substr(0, newVector.find("\r\n\r\n"));
						newVector.erase(0, newVector.find("\r\n\r\n") + 4);
						std::vector<std::string> headerOut = tokenize(headers, theShard);
						if (headerOut.size()) {
							std::string statusLine = headerOut[0];
							headerOut.erase(headerOut.begin());
							std::vector<std::string> status = tokenize(statusLine, theShard, " ");
							theShard->theState = WebSocketState::Connected;
							theShard->getInputBuffer().clear();
							theShard->getInputBuffer().insert(theShard->getInputBuffer().end(), newVector.begin(), newVector.end());
						}
					}
					break;
				case WebSocketState::Connected:
					while (this->parseHeader(theShard)) {
					};
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::handleBuffer()");
			}
			theShard->disconnect();
		}
	}

	bool BaseSocketAgent::parseHeader(SSLClient* theShard) noexcept {
		try {
			std::string newVector = theShard->getInputBuffer();
			if (theShard->getInputBuffer().size() < 4) {
				return false;
			} else {
				uint8_t theValue = theShard->getInputBuffer()[0];
				std::bitset<8> theBits = theValue;
				theBits.set(7, 0);
				WebSocketOpCode opCodeNew = static_cast<WebSocketOpCode>(theBits.to_ulong());
				switch (opCodeNew) {
					case WebSocketOpCode::Op_Pong: {
						uint8_t length01 = theShard->getInputBuffer()[1] & (0 << 7);
						uint32_t payloadStartOffset = 2;
						theShard->getInputBuffer().erase(theShard->getInputBuffer().begin(),
							theShard->getInputBuffer().begin() + payloadStartOffset + length01);
						break;
					}
					case WebSocketOpCode::Op_Ping: {
						if (theShard->getInputBuffer().size() > 2) {
							uint8_t length01 = theShard->getInputBuffer()[1] & (0 << 7);
							uint32_t payloadStartOffset = 2;
							theShard->getInputBuffer().erase(theShard->getInputBuffer().begin(),
								theShard->getInputBuffer().begin() + payloadStartOffset + length01);
						}
						break;
					}
					case WebSocketOpCode::Op_Continuation:
						theShard->getInputBuffer().clear();
						break;
					case WebSocketOpCode::Op_Binary:
					case WebSocketOpCode::Op_Text: {
						uint8_t length01 = theShard->getInputBuffer()[1];
						std::bitset<8> theBits02 = length01;
						theBits02.set(7, 0);
						uint32_t payloadStartOffset = 2;
						uint64_t length02 = theBits02.to_ullong();
						if (length02 == webSocketPayloadLengthMagicLarge) {
							if (theShard->getInputBuffer().size() < 8) {
								return false;
							}
							uint8_t length03 = theShard->getInputBuffer()[2];
							uint8_t length04 = theShard->getInputBuffer()[3];
							length02 = static_cast<uint64_t>((length03 << 8) | length04);
							payloadStartOffset += 2;
						} else if (length02 == webSocketPayloadLengthMagicHuge) {
							if (theShard->getInputBuffer().size() < 10) {
								return false;
							}
							length02 = 0;
							for (uint64_t value = 2, shift = 56; value < 10; ++value, shift -= 8) {
								uint8_t length05 = static_cast<uint8_t>(theShard->getInputBuffer()[value]);
								length02 |= static_cast<uint64_t>(length05) << shift;
							}
							payloadStartOffset += 8;
						}
						if (theShard->getInputBuffer().size() < payloadStartOffset + length02) {
							return false;
						} else {
							std::string leftOverVector{};
							leftOverVector.insert(leftOverVector.begin(), theShard->getInputBuffer().begin() + payloadStartOffset + length02 + 4,
								theShard->getInputBuffer().end());
							std::string newerVector{};
							newerVector.insert(newerVector.begin(), theShard->getInputBuffer().begin() + payloadStartOffset + 4,
								theShard->getInputBuffer().begin() + payloadStartOffset + length02 + 4);
							this->onMessageReceived(( WebSocketSSLShard* )theShard, newerVector);
							theShard->getInputBuffer().clear();
							theShard->getInputBuffer() = leftOverVector;
						}
						return true;
					}
					case WebSocketOpCode::Op_Close: {
						uint16_t close = theShard->getInputBuffer()[2] & 0xff;
						close <<= 8;
						close |= theShard->getInputBuffer()[3] & 0xff;
						this->closeCode = close;
						theShard->getInputBuffer().clear();
						theShard->disconnect();
						return false;
					}
					default: {
						this->closeCode = 1000;
						return false;
					}
				}
			}
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::parseHeader()");
			}
			theShard->disconnect();
		}
		return false;
	}

	BaseSocketAgent::~BaseSocketAgent() noexcept {
		this->theTask->request_stop();
		if (this->theTask->joinable()) {
			this->theTask->join();
		}
	}
};