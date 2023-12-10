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
/// WebSocketEntities.cpp - Source file for the webSocket related classes and structs.
/// May 22, 2022
/// https://github.com/RealTimeChris/discord_core_loader
/// \file WebSocketEntities.cpp

#include <discordcoreloader/WebSocketEntities.hpp>
#include <discordcoreloader/DiscordCoreClient.hpp>

namespace discord_core_loader {

	constexpr uint16_t webSocketMaxPayloadLengthLarge{ 65535 };
	constexpr uint8_t webSocketPayloadLengthMagicLarge{ 126 };
	constexpr uint8_t webSocketPayloadLengthMagicHuge{ 127 };
	constexpr uint8_t maxHeaderSize{ sizeof(uint64_t) + 2 };
	constexpr uint8_t webSocketMaxPayloadLengthSmall{ 125 };
	constexpr uint8_t webSocketFinishBit{ (1u << 7u) };
	constexpr uint8_t webSocketMaskBit{ (1u << 7u) };

	WebSocketSSLShard::WebSocketSSLShard(bool doWePrintErrorsNew, BaseSocketAgent* theAgentNew)
		: SSLClient(doWePrintErrorsNew) {
		this->theAgent = theAgentNew;
	}

	void WebSocketSSLShard::connect(SSL_CTX* contextNew) {
		SSLClient::connect(theAgent->webSocketSSLServerMain->getNewSocket(), contextNew);

		jsonifier::vector<WebSocketSSLShard*> theVector{};
		theVector.emplace_back(this);
		while (authKey == "") {
			theAgent->webSocketSSLServerMain->processIO(theVector);
		}
		jsonifier::string sendString{
			"HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + authKey + "\r\n\r\n"
		};
		this->theAgent->sendMessage(&sendString, this, false);
		auto returnValue = theAgent->webSocketSSLServerMain->processIO(theVector);
		this->theAgent->sendHelloMessage(this);
		returnValue = theAgent->webSocketSSLServerMain->processIO(theVector);
		theVector.clear();
		theVector.emplace_back(this);
		while (shard[0] == -1) {
			returnValue = theAgent->webSocketSSLServerMain->processIO(theVector);
			this->theAgent->handleBuffer(this);
		}
		this->theAgent->sendFinalMessage(this);
		while (!areWeConnected) {
			returnValue = theAgent->webSocketSSLServerMain->processIO(theVector);
			theAgent->handleBuffer(this);
		}
		sendGuilds = true;
	}

	void WebSocketSSLShard::handleBuffer() noexcept {
		theAgent->handleBuffer(this);
	}

	BaseSocketAgent::BaseSocketAgent(DiscordCoreClient* discordCoreClient, std::atomic_bool* doWeQuitNew, bool doWeInstantiateAThread) noexcept {
		webSocketSSLServerMain = std::make_unique<WebSocketSSLServerMain>(discordCoreClient->configParser.getTheData().connectionIp,
			discordCoreClient->configParser.getTheData().connectionPort, discordCoreClient->configParser.getTheData().doWePrintGeneralErrorMessages,
			doWeQuitNew, &discordCoreClient->configParser);
		this->discordCoreClient = discordCoreClient;
		this->doWeQuit = doWeQuitNew;
		this->jsonifier = discordCoreClient->configParser.getTheData();
		if (doWeInstantiateAThread) {
			this->theTask = std::make_unique<std::jthread>([this](std::stop_token theToken) {
				this->run(theToken);
			});
		}
	}

	void BaseSocketAgent::sendMessage(jsonifier::string* dataToSend, WebSocketSSLShard* theShard, bool priority) noexcept {
		try {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessSentMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				std::cout << shiftToBrightBlue() << "Sending WebSocket " + jsonifier::toString(theShard->shard[0]) + jsonifier::string("'s Message: ")
						  << std::endl
						  << *dataToSend << reset();
			}

			theShard->writeData(*dataToSend, priority);
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

	void BaseSocketAgent::sendMessage(etf_serializer&& dataToSend, WebSocketOpCode theOpCode, WebSocketSSLShard* theShard, bool priority) noexcept {
		try {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessSentMessages) {
				std::lock_guard<std::mutex> theLock{ this->discordCoreClient->coutMutex };
				std::cout << shiftToBrightBlue() << "Sending WebSocket " + jsonifier::toString(theShard->shard[0]) + jsonifier::string("'s Message: ")
						  << jsonifier::string{ dataToSend.operator jsonifier::string_base<char, 0Ui64>() } << reset() << reset() << std::endl
						  << std::endl;
			}
			jsonifier::string theVector{ dataToSend.operator jsonifier::string_base<char, 0Ui64>() };
			jsonifier::string header{};
			this->createHeader(header, theVector.size(), theOpCode);
			jsonifier::string theVectorNew{};
			theVectorNew.insert(theVectorNew.begin(), header.begin(), header.end());
			theVectorNew.insert(theVectorNew.end(), theVector.begin(), theVector.end());
			theShard->writeData(theVectorNew, priority);
		} catch (...) {
			if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
				reportException("BaseSocketAgent::sendMessage()");
			}
			theShard->disconnect();
		}
	}

	void BaseSocketAgent::createHeader(jsonifier::string& outBuffer, uint64_t sendLength, WebSocketOpCode opCodeNew) noexcept {
		try {
			outBuffer.pushBack(static_cast<uint8_t>(opCodeNew) | webSocketFinishBit);

			uint32_t indexCount{ 0 };
			if (sendLength <= webSocketMaxPayloadLengthSmall) {
				outBuffer.pushBack(static_cast<uint8_t>(sendLength));
				indexCount = 0;
			} else if (sendLength <= webSocketMaxPayloadLengthLarge) {
				outBuffer.pushBack(static_cast<uint8_t>(webSocketPayloadLengthMagicLarge));
				indexCount = 2;
			} else {
				outBuffer.pushBack(static_cast<uint8_t>(webSocketPayloadLengthMagicHuge));
				indexCount = 8;
			}
			for (int32_t x = indexCount - 1; x >= 0; x--) {
				outBuffer.pushBack(static_cast<uint8_t>(sendLength >> x * 8));
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

	void BaseSocketAgent::sendResumedPayload(WebSocketSSLShard* theShard) noexcept {
		theShard->lastNumberSent++;
		etf_serializer jsonData{};
		jsonData["op"] = static_cast<int8_t>(0);
		jsonData["s"] = theShard->lastNumberSent;
		jsonData["t"] = "RESUMED";
		if (theShard->theMode == WebSocketMode::JSON) {
			theShard->theOpCode = WebSocketOpCode::Op_Text;
		} else {
			theShard->theOpCode = WebSocketOpCode::Op_Binary;
		}
		this->sendMessage(std::move(jsonData), theShard->theOpCode, theShard, true);
	}

	void BaseSocketAgent::generateGuildData() noexcept {
		for (auto& [key, value]: this->theClients) {
			jsonifier::string theString{};
			if (!value->doWeHaveOurGuild) {
				this->jsonifier.randomizeId(theString);
				value->theGuildHolder["d"] = this->jsonifier.JSONIFYGuild(std::move(*this->jsonifier.generateGuild(theString)));
				value->theGuildHolder["op"] = static_cast<int8_t>(0);
				value->theGuildHolder["t"] = "GUILD_CREATE";
				value->doWeHaveOurGuild = true;
			}
		}
	}

	void BaseSocketAgent::sendCreateGuilds(WebSocketSSLShard* theShard) noexcept {
		if (theShard->currentGuildCount < theShard->totalGuildCount && theShard->doWeHaveOurGuild) {
			theShard->currentGuildCount++;
			theShard->lastNumberSent++;
			for (auto& value: theShard->theGuildHolder["d"]["roles"].getArray()) {
				jsonifier::string theString{};
				this->jsonifier.randomizeId(theString);
				value["id"] = std::move(theString);
			}
			for (auto& value: theShard->theGuildHolder["d"]["members"].getArray()) {
				jsonifier::string theString{};
				this->jsonifier.randomizeId(theString);
				value["user"]["id"] = std::move(theString);
			}
			for (auto& value: theShard->theGuildHolder["d"]["channels"].getArray()) {
				jsonifier::string theString{};
				this->jsonifier.randomizeId(theString);
				value["id"] = std::move(theString);
			}
			jsonifier::string theString{};
			this->jsonifier.randomizeId(theString);
			theShard->theGuildHolder["d"]["id"] = theString;
			theShard->theGuildHolder["s"] = theShard->lastNumberSent;
			theShard->theGuildHolder["t"] = "GUILD_CREATE";
			WebSocketMessage theMessage{};
			if (theShard->theMode == WebSocketMode::JSON) {
				theShard->theOpCode = WebSocketOpCode::Op_Text;
			} else {
				theShard->theOpCode = WebSocketOpCode::Op_Binary;
			}
			theMessage.jsonMsg = theShard->theGuildHolder;
			theMessage.theOpCode = theShard->theOpCode;
			theShard->theMessageQueue.emplace_back(std::move(theMessage));
		}
	}

	void BaseSocketAgent::sendReadyMessage(WebSocketSSLShard* theShard) noexcept {
		theShard->lastNumberSent++;
		etf_serializer jsonData{};
		jsonData["op"] = static_cast<int8_t>(0);
		jsonData["s"] = theShard->lastNumberSent;
		jsonData["t"] = "READY";
		jsonifier::string theString{};
		this->jsonifier.randomizeId(theString);
		jsonData["d"]["session_id"] = std::move(theString);
		jsonData["d"]["guilds"];
		int32_t guildSize = this->discordCoreClient->guildQuantity.load() / this->discordCoreClient->totalShardCount.load();
		theShard->totalGuildCount = guildSize;
		if (guildSize > 2500) {
			this->initDisconnect(WebSocketCloseCode::Sharding_Required, theShard);
			return;
		}
		jsonData["d"]["v"] = 10;
		jsonData["d"]["user"] = this->jsonifier.JSONIFYUser(std::move(*this->jsonifier.generateUser()));
		jsonData["d"]["resume_gateway_url"] = "192.168.0.28";
		if (theShard->theMode == WebSocketMode::JSON) {
			theShard->theOpCode = WebSocketOpCode::Op_Text;
		} else {
			theShard->theOpCode = WebSocketOpCode::Op_Binary;
		}
		this->sendMessage(std::move(jsonData), theShard->theOpCode, theShard, true);
	}

	void BaseSocketAgent::sendHelloMessage(WebSocketSSLShard* theShard) noexcept {
		etf_serializer jsonData{};
		jsonData["op"] = static_cast<int8_t>(10);
		jsonData["d"]["heartbeat_interval"] = static_cast<uint16_t>(this->heartbeatInterval);
		if (theShard->theMode == WebSocketMode::JSON) {
			theShard->theOpCode = WebSocketOpCode::Op_Text;
		} else {
			theShard->theOpCode = WebSocketOpCode::Op_Binary;
		}
		this->sendMessage(std::move(jsonData), theShard->theOpCode, theShard, true);
	}

	void BaseSocketAgent::sendHeartBeat(WebSocketSSLShard* theShard) noexcept {
		etf_serializer jsonData{};
		jsonData["op"] = static_cast<int8_t>(11);
		jsonData["d"] = theShard->lastNumberSent;
		if (theShard->theMode == WebSocketMode::JSON) {
			theShard->theOpCode = WebSocketOpCode::Op_Text;
		} else {
			theShard->theOpCode = WebSocketOpCode::Op_Binary;
		}
		this->sendMessage(std::move(jsonData), theShard->theOpCode, theShard, true);
	}

	jsonifier::vector<jsonifier::string> BaseSocketAgent::tokenize(const jsonifier::string& dataIn, WebSocketSSLShard* theShard, const jsonifier::string& separator) noexcept {
		try {
			size_t value{ 0 };
			jsonifier::vector<jsonifier::string> dataOut{};
			while ((value = dataIn.findFirstNotOf(separator, value)) != jsonifier::string::npos) {
				auto output = dataIn.find(separator, value);
				dataOut.emplace_back(dataIn.substr(value, output - value));
				if (dataIn.substr(value, output - value).find("GET") != jsonifier::string::npos) {
					jsonifier::string newString = dataIn.substr(dataIn.find("v=") + 2, dataIn.size() - dataIn.find("v=") + 2);
					int8_t index{ 0 };
					for (auto& value02: newString) {
						index++;
						if (value != isdigit(value02)) {
							index++;
							break;
						}
					}
					int32_t version = jsonifier::strToInt64(newString.substr(0, index));
					if (version != 10) {
						this->initDisconnect(WebSocketCloseCode::Invalid_API_Version, theShard);
					}
				}
				if (dataIn.substr(value, output - value).find("Sec-WebSocket-Key:") != jsonifier::string::npos) {
					theShard->authKey = dataOut[dataOut.size() - 1].substr(
						dataOut[dataOut.size() - 1].find("Sec-WebSocket-Key: ") + jsonifier::string{ "Sec-WebSocket-Key: " }.size());
				}
				if (dataIn.substr(value, output - value).find("json") != jsonifier::string::npos) {
					theShard->theMode = WebSocketMode::JSON;
					theShard->theOpCode = WebSocketOpCode::Op_Text;
				} else if (dataIn.substr(value, output - value).find("etf") != jsonifier::string::npos) {
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
			return jsonifier::vector<jsonifier::string>{};
		}
	}

	void BaseSocketAgent::initDisconnect(WebSocketCloseCode reason, WebSocketSSLShard* theShard) noexcept {
		if (theShard != 0) {
			jsonifier::string theString{};
			theString.pushBack(static_cast<int8_t>(WebSocketOpCode::Op_Close) | static_cast<int8_t>(webSocketFinishBit));
			theString.pushBack(0);
			theString.pushBack(static_cast<uint16_t>(reason) >> 8);
			theString.pushBack(static_cast<int8_t>(reason) & 0xff);
			while (theShard->theMessageQueue.size() > 0) {
				theShard->theMessageQueue.pop_front();
			}
			WebSocketMessage theMessage{};
			theMessage.stringMsg = theString;
			theMessage.theOpCode = WebSocketOpCode::Op_Close;
			theShard->theMessageQueue.push_front(theMessage);
		}
	}

	void BaseSocketAgent::sendFinalMessage(WebSocketSSLShard* theShard) noexcept {
		if (theShard->outputBuffers.size() == 0) {
			if (theShard->theMessageQueue.size() > 0) {
				WebSocketMessage newMessage = std::move(theShard->theMessageQueue.front());
				theShard->theMessageQueue.pop_front();
				if (newMessage.stringMsg.size() > 0) {
					this->sendMessage(&newMessage.stringMsg, theShard, false);
				} else {
					this->sendMessage(std::move(newMessage.jsonMsg), newMessage.theOpCode, theShard, false);
				}
			}
		}
	}

	void BaseSocketAgent::run(std::stop_token theToken) noexcept {
		try {
			while (!theToken.stop_requested() && !this->doWeQuit->load()) {
				if (this->theClients.size() > 0) {
					jsonifier::vector<WebSocketSSLShard*> theVector{};
					for (auto& [key, value]: this->theClients) {
						if (value && value->areWeStillConnected() && value->areWeConnected) {
							theVector.emplace_back(value.get());
						} else {
							value->connect(webSocketSSLServerMain->context.operator SSL_CTX*());
						}
					}
					if (theVector.size() == 0) {
						std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
					}
					theVector = this->webSocketSSLServerMain->processIO(theVector);
					for (auto& value: theVector) {
						if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketErrorMessages) {
							std::unique_lock theLockTwo{ this->discordCoreClient->coutMutex };
							std::cout << shiftToBrightRed() << "Connection lost for WebSocket [" << jsonifier::toString(value->shard[0]) << ","
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

	void BaseSocketAgent::sendGuildMemberChunks(WebSocketSSLShard*) noexcept {
	}

	void BaseSocketAgent::onMessageReceived(WebSocketSSLShard* theShard, jsonifier::string& theString) noexcept {
		try {
			jsonifier::string_view_base<char> payload{ theString };
			bool returnValue{ false };
			WebSocketMessageData theMessage{};
			if (theString.size() > 0) {
				returnValue = true;

				if (theShard->theOpCode == WebSocketOpCode::Op_Binary) {
					try {
						payload = etf_parser::parseEtfToJson(theString);
						parser.parseJson(theMessage, payload);
						for (auto& value: parser.getErrors()) {
							std::cout << "Error: " << value << std::endl;
						}

					} catch (...) {
						returnValue = false;
					}
				} else {
					payload = theString;
					parser.parseJson(theMessage, payload);
					for (auto& value: parser.getErrors()) {
						std::cout << "Error: " << value << std::endl;
					}
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
						for (auto& value: parser.getErrors()) {
							std::cout << "Error: " << value << std::endl;
						}
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
						  << "Message received from WebSocket [" + jsonifier::toString(theShard->shard[0]) + "," + jsonifier::toString(theShard->shard[1]) +
						"]" + jsonifier::string(": ")
						  << payload << std::endl;
				if (this->discordCoreClient->configParser.getTheData().doWePrintWebSocketSuccessReceiveMessages) {
					
				}
			}
		} catch (...) {
			reportException("BaseSocektAgent::onMessageReceived()");
			return;
		}
	}

	void BaseSocketAgent::handleBuffer(WebSocketSSLShard* theShard) noexcept {
		try {
			jsonifier::string newVector = theShard->getInputBuffer();
			switch (theShard->theState) {
				case WebSocketState::Initializing:
					if (newVector.find("\r\n\r\n") != jsonifier::string::npos) {
						jsonifier::string headers = newVector.substr(0, newVector.find("\r\n\r\n"));
						newVector.erase(newVector.find("\r\n\r\n") + 4);
						jsonifier::vector<jsonifier::string> headerOut = tokenize(headers, theShard);
						if (headerOut.size()) {
							jsonifier::string statusLine = headerOut[0];
							headerOut.erase(headerOut.begin());
							jsonifier::vector<jsonifier::string> status = tokenize(statusLine, theShard, " ");
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

	bool BaseSocketAgent::parseHeader(WebSocketSSLShard* theShard) noexcept {
		try {
			jsonifier::string newVector = theShard->getInputBuffer();
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
						theShard->getInputBuffer().erase(theShard->getInputBuffer().begin() + payloadStartOffset + length01);
						break;
					}
					case WebSocketOpCode::Op_Ping: {
						if (theShard->getInputBuffer().size() > 2) {
							uint8_t length01 = theShard->getInputBuffer()[1] & (0 << 7);
							uint32_t payloadStartOffset = 2;
							theShard->getInputBuffer().erase(theShard->getInputBuffer().begin() + payloadStartOffset + length01);
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
							jsonifier::string leftOverVector{};
							leftOverVector.insert(leftOverVector.begin(), theShard->getInputBuffer().begin() + payloadStartOffset + length02 + 4,
								theShard->getInputBuffer().end());
							jsonifier::string newerVector{};
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

	void BaseSocketAgent::connectShard(std::unique_ptr<WebSocketSSLShard>& shard) noexcept {

	}

	BaseSocketAgent::~BaseSocketAgent() noexcept {
		this->theTask->request_stop();
		if (this->theTask->joinable()) {
			this->theTask->join();
		}
	}
};