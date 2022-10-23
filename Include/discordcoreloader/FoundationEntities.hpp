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
/// FoundationEntities.hpp - Header for all of the Discord/Support API data
/// May 22, 2022
/// https://discordcoreapi.com
/// \file FoundationEntities.hpp

#pragma once

#pragma warning(push)
#pragma warning(disable : 4275)
#pragma warning(disable : 4244)
#pragma warning(disable : 4251)
#pragma warning(disable : 4996)
#pragma warning(disable : 4706)
#pragma warning(disable : 4100)
#pragma warning(disable : 4459)
#pragma warning(disable : 4245)

#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#ifndef WINRT_LEAN_AND_MEAN
		#define WINRT_LEAN_AND_MEAN
	#endif
	#include <WinSock2.h>
#else
	#include <cstdint>
	#include <cstring>
inline uint64_t ntohll(uint64_t x) {
	uint8_t data[8]{};
	memcpy(&data, &(x), sizeof(x));
	uint64_t theValue{};
	for (uint32_t y = 0; y < sizeof(uint64_t); ++y) {
		theValue |= data[y] << 8 * (sizeof(uint64_t) - y - 1);
	}
	return theValue;
}
	#include <arpa/inet.h>
	#include <pthread.h>
	#include <sys/time.h>
	#include <time.h>
	#include <ctime>
#endif

#include <condition_variable>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <functional>
#include <simdjson.h>
#include <semaphore>
#include <coroutine>
#include <concepts>
#include <iostream>
#include <fstream>
#include <bitset>
#include <memory>
#include <random>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <map>

#ifdef max
	#undef max
#endif

#ifdef min
	#undef min
#endif

/**
 * \defgroup main_endpoints Main Endpoints
 * \brief For all of the Discord API's endpoints.
 */

/**
 * \defgroup voice_connection Voice Connection
 * \brief For all of the voice connection related stuff.
 */

/**
 * \defgroup discord_events Discord Events
 * \brief For all of the events that could be sent by the Discord API.
 */

/**
 * \defgroup utilities Utilities
 * \brief For utility classes/functions.
 */

/**
 * \defgroup foundation_entities Foundation Entities
 * \brief For all of the building blocks of the main endpoints.
 */

/**
 * \addtogroup foundation_entities
 * @{
 */
/// The main namespace for this library. \brief The main namespace for this
/// library.
namespace DiscordCoreLoader {

	struct RecurseThroughMessagePagesData;
	struct DeleteInteractionResponseData;
	struct DeleteFollowUpMessageData;
	struct OnInteractionCreationData;
	struct GetGuildMemberRolesData;
	struct BaseFunctionArguments;
	struct HttpWorkloadData;
	struct GetRolesData;
	struct CommandData;
	struct File;

	class CreateEphemeralInteractionResponseData;
	class CreateDeferredInteractionResponseData;
	class CreateEphemeralFollowUpMessageData;
	class CreateInteractionResponseData;
	class EditInteractionResponseData;
	class CreateFollowUpMessageData;
	class RespondToInputEventData;
	class EditFollowUpMessageData;
	class SelectMenuCollector;
	class DiscordCoreClient;
	class CreateMessageData;
	class VoiceConnection;
	class EditMessageData;
	class ButtonCollector;
	class ModalCollector;
	class Interactions;
	class EventManager;
	class EventHandler;
	class GuildMembers;
	class GuildMember;
	class ChannelData;
	class InputEvents;
	class EventWaiter;
	class SendDMData;
	class Reactions;
	class Messages;
	class WebHooks;
	class SongAPI;
	class BotUser;
	class Guilds;
	class Roles;
	class Test;

	enum class WebSocketOpCode : int8_t { Op_Continuation = 0x00, Op_Text = 0x01, Op_Binary = 0x02, Op_Close = 0x08, Op_Ping = 0x09, Op_Pong = 0x0a };

	template<typename TimeType> class StopWatch {
	  public:
		StopWatch() = delete;

		StopWatch<TimeType>& operator=(const StopWatch<TimeType>& data) {
			this->maxNumberOfMs.store(data.maxNumberOfMs.load());
			this->startTime.store(data.startTime.load());
			return *this;
		}

		StopWatch(const StopWatch<TimeType>& data) {
			*this = data;
		}

		StopWatch(TimeType maxNumberOfMsNew) {
			this->maxNumberOfMs.store(maxNumberOfMsNew.count());
			this->startTime.store(static_cast<uint64_t>(std::chrono::duration_cast<TimeType>(std::chrono::system_clock::now().time_since_epoch()).count()));
		}

		uint64_t totalTimePassed() {
			uint64_t currentTime = static_cast<uint64_t>(std::chrono::duration_cast<TimeType>(std::chrono::system_clock::now().time_since_epoch()).count());
			uint64_t elapsedTime = currentTime - this->startTime.load();
			return elapsedTime;
		}

		bool hasTimePassed() {
			uint64_t currentTime = static_cast<uint64_t>(std::chrono::duration_cast<TimeType>(std::chrono::system_clock::now().time_since_epoch()).count());
			uint64_t elapsedTime = currentTime - this->startTime.load();
			if (elapsedTime >= this->maxNumberOfMs.load()) {
				return true;
			} else {
				return false;
			}
		}

		void resetTimer() {
			this->startTime.store(static_cast<uint64_t>(std::chrono::duration_cast<TimeType>(std::chrono::system_clock::now().time_since_epoch()).count()));
		}

	  protected:
		std::atomic_uint64_t maxNumberOfMs{ 0 };
		std::atomic_uint64_t startTime{ 0 };
	};

	template<typename ReturnType> ReturnType reverseByteOrder(const ReturnType net) {
		switch (sizeof(ReturnType)) {
			case 1: {
				return net;
			}
			case 2: {
				return ntohs(static_cast<int16_t>(net));
			}
			case 4: {
				return ntohl(static_cast<uint32_t>(net));
			}
			case 8: {
				return ntohll(static_cast<uint64_t>(net));
			}
		}
		return ReturnType{};
	}

	template<typename ReturnType> void storeBits(std::string& to, ReturnType num) {
		const uint8_t byteSize{ 8 };
		ReturnType newValue = reverseByteOrder<ReturnType>(num);
		for (uint32_t x = 0; x < sizeof(ReturnType); ++x) {
			to.push_back(static_cast<uint8_t>(newValue >> (byteSize * x)));
		}
	}

	constexpr uint8_t formatVersion{ 131 };

	enum class EtfType : uint8_t {
		New_Float_Ext = 70,
		Small_Integer_Ext = 97,
		Integer_Ext = 98,
		Atom_Ext = 100,
		Nil_Ext = 106,
		String_Ext = 107,
		List_Ext = 108,
		Binary_Ext = 109,
		Small_Big_Ext = 110,
		Map_Ext = 116,
	};

	template<typename T>
	concept IsEnum = std::is_enum<T>::value;

	struct EnumConverter {
		template<IsEnum EnumType> EnumConverter& operator=(std::vector<EnumType> data) {
			for (auto& value: data) {
				this->vector.emplace_back(std::move(static_cast<uint64_t>(value)));
			}
			return *this;
		};

		template<IsEnum EnumType> EnumConverter(std::vector<EnumType> data) {
			*this = data;
		};

		template<IsEnum EnumType> EnumConverter& operator=(EnumType data) {
			this->integer = static_cast<uint64_t>(data);
			return *this;
		};

		template<IsEnum EnumType> EnumConverter(EnumType data) {
			*this = data;
		};

		operator std::vector<uint64_t>() const noexcept;

		operator uint64_t() const noexcept;

		bool isItAVector() const noexcept;

	  protected:
		std::vector<uint64_t> vector{};
		bool vectorType{ false };
		uint64_t integer{};
	};

	enum class JsonType : int8_t { Object = 1, Array = 2, String = 3, Float = 4, Uint64 = 5, Int64 = 6, Bool = 7, Null = 8 };

	enum class JsonifierSerializeType { Etf = 0, Json = 1 };

	class Jsonifier;

	template<typename T>
	concept IsConvertibleToJsonifier = std::convertible_to<T, Jsonifier>;

	class Jsonifier {
	  public:
		using MapAllocatorType = std::allocator<std::pair<const std::string, Jsonifier>>;
		template<typename OTy> using AllocatorType = std::allocator<OTy>;
		template<typename OTy> using AllocatorTraits = std::allocator_traits<AllocatorType<OTy>>;
		using ObjectType = std::map<std::string, Jsonifier, std::less<>, MapAllocatorType>;
		using ArrayType = std::vector<Jsonifier, AllocatorType<Jsonifier>>;
		using StringType = std::string;
		using FloatType = double;
		using UintType = uint64_t;
		using IntType = int64_t;
		using BoolType = bool;

		union JsonValue {
			JsonValue() noexcept = default;
			JsonValue& operator=(JsonValue&&) noexcept = delete;
			JsonValue(JsonValue&&) noexcept = delete;
			JsonValue& operator=(const JsonValue&) noexcept = delete;
			JsonValue(const JsonValue&) noexcept = delete;
			ObjectType* object;
			StringType* string;
			ArrayType* array;
			FloatType numberDouble;
			UintType numberUint;
			IntType numberInt;
			BoolType boolean;
		};

		Jsonifier() noexcept = default;

		template<IsConvertibleToJsonifier OTy> Jsonifier& operator=(std::vector<OTy>&& data) noexcept {
			this->setValue(JsonType::Array);
			for (auto& value: data) {
				this->jsonValue.array->push_back(std::move(value));
			}
			return *this;
		}

		template<IsConvertibleToJsonifier OTy> Jsonifier(std::vector<OTy>&& data) noexcept {
			*this = std::move(data);
		}

		template<IsConvertibleToJsonifier OTy> Jsonifier& operator=(std::vector<OTy>& data) noexcept {
			this->setValue(JsonType::Array);
			for (auto& value: data) {
				this->jsonValue.array->push_back(value);
			}
			return *this;
		}

		template<IsConvertibleToJsonifier OTy> Jsonifier(std::vector<OTy>& data) noexcept {
			*this = data;
		}

		template<IsConvertibleToJsonifier KTy, IsConvertibleToJsonifier OTy> Jsonifier& operator=(std::unordered_map<KTy, OTy>&& data) noexcept {
			this->setValue(JsonType::Object);
			for (auto& [key, value]: data) {
				(*this->jsonValue.object)[key] = std::move(value);
			}
			return *this;
		}

		template<IsConvertibleToJsonifier KTy, IsConvertibleToJsonifier OTy> Jsonifier(std::unordered_map<KTy, OTy>&& data) noexcept {
			*this = std::move(data);
		};

		template<IsConvertibleToJsonifier KTy, IsConvertibleToJsonifier OTy> Jsonifier& operator=(std::unordered_map<KTy, OTy>& data) noexcept {
			this->setValue(JsonType::Object);
			for (auto& [key, value]: data) {
				(*this->jsonValue.object)[key] = value;
			}
			return *this;
		}

		template<IsConvertibleToJsonifier KTy, IsConvertibleToJsonifier OTy> Jsonifier(std::unordered_map<KTy, OTy>& data) noexcept {
			*this = data;
		};

		template<IsConvertibleToJsonifier KTy, IsConvertibleToJsonifier OTy> Jsonifier& operator=(std::map<KTy, OTy>&& data) noexcept {
			this->setValue(JsonType::Object);
			for (auto& [key, value]: data) {
				(*this->jsonValue.object)[key] = std::move(value);
			}
			return *this;
		}

		template<IsConvertibleToJsonifier KTy, IsConvertibleToJsonifier OTy> Jsonifier(std::map<KTy, OTy>&& data) noexcept {
			*this = std::move(data);
		};

		template<IsConvertibleToJsonifier KTy, IsConvertibleToJsonifier OTy> Jsonifier& operator=(std::map<KTy, OTy>& data) noexcept {
			this->setValue(JsonType::Object);
			for (auto& [key, value]: data) {
				(*this->jsonValue.object)[key] = value;
			}
			return *this;
		}

		template<IsConvertibleToJsonifier KTy, IsConvertibleToJsonifier OTy> Jsonifier(std::map<KTy, OTy>& data) noexcept {
			*this = data;
		};

		template<IsEnum T> Jsonifier& operator=(T data) noexcept {
			this->jsonValue.numberUint = static_cast<uint64_t>(data);
			this->type = JsonType::Uint64;
			return *this;
		}

		template<IsEnum T> Jsonifier(T data) noexcept {
			*this = data;
		}

		Jsonifier& operator=(Jsonifier&& data) noexcept;

		Jsonifier(Jsonifier&& data) noexcept;

		Jsonifier& operator=(const Jsonifier& data) noexcept;

		Jsonifier(const Jsonifier& data) noexcept;

		operator std::string&&() noexcept;

		operator std::string() noexcept;

		void refreshString(JsonifierSerializeType OpCode);

		Jsonifier& operator=(EnumConverter&& data) noexcept;
		Jsonifier(EnumConverter&& data) noexcept;

		Jsonifier& operator=(const EnumConverter& data) noexcept;
		Jsonifier(const EnumConverter& data) noexcept;

		Jsonifier& operator=(std::string&& data) noexcept;
		Jsonifier(std::string&& data) noexcept;

		Jsonifier& operator=(const std::string& data) noexcept;
		Jsonifier(const std::string& data) noexcept;

		Jsonifier& operator=(const char* data) noexcept;
		Jsonifier(const char* data) noexcept;

		Jsonifier& operator=(double data) noexcept;
		Jsonifier(double data) noexcept;

		Jsonifier& operator=(float data) noexcept;
		Jsonifier(float data) noexcept;

		Jsonifier& operator=(uint64_t data) noexcept;
		Jsonifier(uint64_t data) noexcept;

		Jsonifier& operator=(uint32_t data) noexcept;
		Jsonifier(uint32_t data) noexcept;

		Jsonifier& operator=(uint16_t data) noexcept;
		Jsonifier(uint16_t data) noexcept;

		Jsonifier& operator=(uint8_t data) noexcept;
		Jsonifier(uint8_t data) noexcept;

		Jsonifier& operator=(int64_t data) noexcept;
		Jsonifier(int64_t data) noexcept;

		Jsonifier& operator=(int32_t data) noexcept;
		Jsonifier(int32_t data) noexcept;

		Jsonifier& operator=(int16_t data) noexcept;
		Jsonifier(int16_t data) noexcept;

		Jsonifier& operator=(int8_t data) noexcept;
		Jsonifier(int8_t data) noexcept;

		Jsonifier& operator=(bool data) noexcept;
		Jsonifier(bool data) noexcept;

		Jsonifier& operator=(JsonType TypeNew) noexcept;
		Jsonifier(JsonType type) noexcept;

		Jsonifier& operator=(std::nullptr_t) noexcept;
		Jsonifier(std::nullptr_t data) noexcept;

		Jsonifier& operator[](typename ObjectType::key_type key);

		Jsonifier& operator[](uint64_t index);

		template<typename T> T& get();

		JsonType getType() noexcept;

		void emplaceBack(Jsonifier&& data) noexcept;
		void emplaceBack(Jsonifier& data) noexcept;

		~Jsonifier() noexcept;

	  private:
		JsonType type{ JsonType::Null };
		JsonValue jsonValue{};
		std::string string{};

		void serializeJsonToEtfString(const Jsonifier* dataToParse);

		void serializeJsonToJsonString(const Jsonifier* dataToParse);

		void writeJsonObject(const ObjectType& ObjectNew);

		void writeJsonArray(const ArrayType& Array);

		void writeJsonString(const StringType& StringNew);

		void writeJsonFloat(const FloatType x);

		template<typename NumberType,
			std::enable_if_t<std::is_integral<NumberType>::value || std::is_same<NumberType, uint64_t>::value || std::is_same<NumberType, int64_t>::value, int> = 0>
		void writeJsonInt(NumberType Int) {
			auto IntNew = std::to_string(Int);
			this->writeString(IntNew.data(), IntNew.size());
		}

		void writeJsonBool(const BoolType ValueNew);

		void writeJsonNull();

		void writeEtfObject(const ObjectType& jsonData);

		void writeEtfArray(const ArrayType& jsonData);

		void writeEtfString(const StringType& jsonData);

		void writeEtfUint(const UintType jsonData);

		void writeEtfInt(const IntType jsonData);

		void writeEtfFloat(const FloatType jsonData);

		void writeEtfBool(const BoolType jsonData);

		void writeEtfNull();

		void writeString(const char* data, std::size_t length);

		void writeCharacter(const char Char);

		void appendStringExt(const std::string& bytes, const uint16_t length);

		void appendBinaryExt(const std::string& bytes, const uint32_t sizeNew);

		void appendUnsignedLongLong(const uint64_t value);

		void appendNewFloatExt(const double FloatValue);

		void appendSmallIntegerExt(const uint8_t value);

		void appendListHeader(const uint32_t sizeNew);

		void appendMapHeader(const uint32_t sizeNew);

		void appendIntegerExt(const uint32_t value);

		void appendBool(bool data);

		void appendVersion();

		void appendNilExt();

		void appendNil();

		void setValue(JsonType TypeNew);

		void destroy() noexcept;

		friend bool operator==(const Jsonifier& lhs, const Jsonifier& rhs);
	};

	template<typename ReturnType, typename... ArgTypes> class Event;
	template<typename ReturnType, typename... ArgTypes> class EventDelegate;

	template<typename ObjectType> class ReferenceCountingPtr {
	  public:
		class ObjectWrapper {
		  public:
			ObjectWrapper& operator=(ObjectType* other) {
				this->thePtr = other;
				return *this;
			}

			ObjectWrapper(ObjectType* other) {
				*this = other;
			}

			operator ObjectType*() {
				return this->thePtr;
			}

			ObjectWrapper() = default;

			ObjectType* get() {
				return this->thePtr;
			}

			void incrementCount() const {
				this->refCount++;
			}

			void release() const {
				assert(this->refCount > 0);
				this->refCount -= 1;
				if (this->refCount == 0) {
					delete this;
				};
			}

			virtual ~ObjectWrapper() = default;

		  protected:
			ObjectType* thePtr{ nullptr };
			mutable int32_t refCount{ 0 };
		};

		ReferenceCountingPtr& operator=(ObjectType* ptr) {
			if (this->thePtr) {
				this->thePtr->release();
			}
			this->thePtr = new ObjectWrapper{ ptr };
			if (this->thePtr) {
				this->thePtr->incrementCount();
			}
			return *this;
		}

		ReferenceCountingPtr(ObjectType* ptr = nullptr) {
			*this = ptr;
		}

		ReferenceCountingPtr& operator=(const ReferenceCountingPtr& ptr) {
			this->thePtr = ptr.thePtr;
			return *this;
		}

		ReferenceCountingPtr(const ReferenceCountingPtr& ptr) {
			*this = ptr;
		}

		ReferenceCountingPtr(nullptr_t){};

		ObjectType* operator->() const {
			return this->thePtr->get();
		}

		ObjectType& operator*() const {
			return *this->thePtr->get();
		}

		ObjectType* get() const {
			return this->thePtr->get();
		}

		~ReferenceCountingPtr() {
			if (this->thePtr) {
				this->thePtr->release();
			}
		}

	  protected:
		ObjectWrapper* thePtr{ nullptr };
	};

	/**
	 * \addtogroup utilities
	 * @{
	 */

	template<typename ObjectType>
	concept Copyable = std::copyable<ObjectType>;

	/// A messaging block for data-structures. \brief A messaging block for data-structures.
	/// \tparam ObjectType The type of object that will be sent over the message block.
	template<Copyable ObjectType> class UnboundedMessageBlock {
	  public:
		UnboundedMessageBlock<ObjectType>& operator=(UnboundedMessageBlock<ObjectType>&& other) {
			if (this != &other) {
				this->theArray = std::move(other.theArray);
				other.theArray = std::queue<ObjectType>{};
			}
			return *this;
		}

		UnboundedMessageBlock(UnboundedMessageBlock<ObjectType>&& other) {
			*this = std::move(other);
		}

		UnboundedMessageBlock<ObjectType>& operator=(const UnboundedMessageBlock<ObjectType>&) = delete;

		UnboundedMessageBlock(const UnboundedMessageBlock<ObjectType>&) = delete;

		UnboundedMessageBlock<ObjectType>& operator=(UnboundedMessageBlock<ObjectType>&) = delete;

		UnboundedMessageBlock(UnboundedMessageBlock<ObjectType>&) = delete;

		UnboundedMessageBlock() = default;

		/// Sends an object of type ObjectType to the "recipient". \brief Sends an object of type ObjectType to the "recipient".
		/// \param theObject An object of ObjectType.
		void send(const ObjectType&& theObject) {
			this->theArray.push(theObject);
		}

		/// Sends an object of type ObjectType to the "recipient". \brief Sends an object of type ObjectType to the "recipient".
		/// \param theObject An object of ObjectType.
		void send(ObjectType&& theObject) {
			this->theArray.push(std::move(theObject));
		}

		/// Sends an object of type ObjectType to the "recipient". \brief Sends an object of type ObjectType to the "recipient".
		/// \param theObject An object of ObjectType.
		void send(const ObjectType& theObject) {
			ObjectType newValue = theObject;
			this->theArray.push(newValue);
		}

		/// Sends an object of type ObjectType to the "recipient". \brief Sends an object of type ObjectType to the "recipient".
		/// \param theObject An object of ObjectType.
		void send(ObjectType& theObject) {
			ObjectType newValue = theObject;
			this->theArray.push(newValue);
		}

		/// Clears the contents of the messaging block. \brief Clears the contents of the messaging block.
		void clearContents() {
			this->theArray = std::queue<ObjectType>{};
		}

		/// Tries to receive an object of type ObjectType to be placed into a reference. \brief Tries to receive an object of type ObjectType to be placed into a reference.
		/// \param theObject A reference of type ObjectType for placing the potentially received object.
		/// \returns A bool, denoting whether or not we received an object.
		bool tryReceive(ObjectType& theObject) {
			if (this->theArray.size() == 0) {
				return false;
			} else {
				theObject = this->theArray.front();
				this->theArray.pop();
				return true;
			}
		}

	  protected:
		std::queue<ObjectType> theArray{};
	};

	/// A thread-safe messaging block for data-structures. \brief A thread-safe messaging block for data-structures.
	/// \tparam ObjectType The type of object that will be sent over the message block.
	template<Copyable ObjectType> class TSUnboundedMessageBlock {
	  public:
		TSUnboundedMessageBlock<ObjectType>& operator=(TSUnboundedMessageBlock<ObjectType>&& other) noexcept {
			if (this != &other) {
				this->theArray = std::move(other.theArray);
				other.theArray = std::queue<ObjectType>{};
			}
			return *this;
		}

		TSUnboundedMessageBlock(TSUnboundedMessageBlock<ObjectType>&& other) noexcept {
			*this = std::move(other);
		}

		TSUnboundedMessageBlock<ObjectType>& operator=(const TSUnboundedMessageBlock<ObjectType>&) = delete;

		TSUnboundedMessageBlock(const TSUnboundedMessageBlock<ObjectType>&) = delete;

		TSUnboundedMessageBlock<ObjectType>& operator=(TSUnboundedMessageBlock<ObjectType>&) = delete;

		TSUnboundedMessageBlock(TSUnboundedMessageBlock<ObjectType>&) = delete;

		TSUnboundedMessageBlock() = default;

		/// Sends an object of type ObjectType to the "recipient". \brief Sends an object of type ObjectType to the "recipient".
		/// \param theObject An object of ObjectType.
		void send(const ObjectType&& theObject) {
			std::lock_guard<std::mutex> theLock{ this->accessMutex };
			this->theArray.push(theObject);
		}

		/// Sends an object of type ObjectType to the "recipient". \brief Sends an object of type ObjectType to the "recipient".
		/// \param theObject An object of ObjectType.
		void send(ObjectType&& theObject) {
			std::lock_guard<std::mutex> theLock{ this->accessMutex };
			this->theArray.push(std::move(theObject));
		}

		/// Sends an object of type ObjectType to the "recipient". \brief Sends an object of type ObjectType to the "recipient".
		/// \param theObject An object of ObjectType.
		void send(const ObjectType& theObject) {
			std::lock_guard<std::mutex> theLock{ this->accessMutex };
			ObjectType newValue = theObject;
			this->theArray.push(newValue);
		}

		/// Sends an object of type ObjectType to the "recipient". \brief Sends an object of type ObjectType to the "recipient".
		/// \param theObject An object of ObjectType.
		void send(ObjectType& theObject) {
			std::lock_guard<std::mutex> theLock{ this->accessMutex };
			ObjectType newValue = theObject;
			this->theArray.push(newValue);
		}

		/// Clears the contents of the messaging block. \brief Clears the contents of the messaging block.
		void clearContents() {
			std::lock_guard<std::mutex> theLock{ this->accessMutex };
			this->theArray = std::queue<ObjectType>{};
		}

		/// Tries to receive an object of type ObjectType to be placed into a reference. \brief Tries to receive an object of type ObjectType to be placed into a reference.
		/// \param theObject A reference of type ObjectType for placing the potentially received object.
		/// \returns A bool, denoting whether or not we received an object.
		bool tryReceive(ObjectType& theObject) {
			std::lock_guard<std::mutex> theLock{ this->accessMutex };
			if (this->theArray.size() == 0) {
				return false;
			} else {
				theObject = this->theArray.front();
				this->theArray.pop();
				return true;
			}
		}

	  protected:
		std::queue<ObjectType> theArray{};
		std::mutex accessMutex{};
	};

	/**@}*/

	struct RequestGuildMembersData {
		std::vector<std::string> userIds{};///< Snowflake or array of snowflakes used to specify which users you wish to fetch one of query or user_ids.
		bool presences{ false };///< Used to specify if we want the presences of the matched members.
		std::string guildId{};///< Id of the guild to get members for.
		std::string nonce{};///< Nonce to identify the Guild Members Chunk response.
		std::string query{};///< std::string string that username starts with, or an empty string to return all members. one of query or user_ids.
		int32_t limit{ 0 };///< Maximum number of members to send matching the query; a limit of 0 can be used with an empty string query to return all members.
	};

	class Time {
	  public:
		Time(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second) {
			this->second = second;
			this->minute = minute;
			this->month = month;
			this->year = year;
			this->hour = hour;
			this->day = day;
		};

		uint64_t getTime() {
			uint64_t theValue{};
			for (int32_t x = 1970; x < this->year; x++) {
				theValue += this->secondsInJan;
				theValue += this->secondsInFeb;
				theValue += this->secondsInMar;
				theValue += this->secondsInApr;
				theValue += this->secondsInMay;
				theValue += this->secondsInJun;
				theValue += this->secondsInJul;
				theValue += this->secondsInAug;
				theValue += this->secondsInSep;
				theValue += this->secondsInOct;
				theValue += this->secondsInNov;
				theValue += this->secondsInDec;
				if (x % 4 == 0) {
					theValue += this->secondsPerDay;
				}
			}
			if (this->month > 0) {
				theValue += static_cast<uint64_t>((this->day - 1) * this->secondsPerDay);
				theValue += static_cast<uint64_t>(this->hour * this->secondsPerHour);
				theValue += static_cast<uint64_t>(this->minute * this->secondsPerMinute);
				theValue += this->second;
			}
			if (this->month > 1) {
				theValue += this->secondsInJan;
			}
			if (this->month > 2) {
				theValue += this->secondsInFeb;
			}
			if (this->month > 3) {
				theValue += this->secondsInMar;
			}
			if (this->month > 4) {
				theValue += this->secondsInApr;
			}
			if (this->month > 5) {
				theValue += this->secondsInMay;
			}
			if (this->month > 6) {
				theValue += this->secondsInJun;
			}
			if (this->month > 7) {
				theValue += this->secondsInJul;
			}
			if (this->month > 8) {
				theValue += this->secondsInAug;
			}
			if (this->month > 9) {
				theValue += this->secondsInSep;
			}
			if (this->month > 10) {
				theValue += this->secondsInOct;
			}
			if (this->month > 11) {
				theValue += this->secondsInNov;
			}
			return theValue;
		}

	  protected:
		uint64_t year{ 0 };
		uint64_t month{ 0 };
		uint64_t day{ 0 };
		uint64_t hour{ 0 };
		uint64_t minute{ 0 };
		uint64_t second{ 0 };
		const int32_t secondsInJan{ 31 * 24 * 60 * 60 };
		const int32_t secondsInFeb{ 28 * 24 * 60 * 60 };
		const int32_t secondsInMar{ 31 * 24 * 60 * 60 };
		const int32_t secondsInApr{ 30 * 24 * 60 * 60 };
		const int32_t secondsInMay{ 31 * 24 * 60 * 60 };
		const int32_t secondsInJun{ 30 * 24 * 60 * 60 };
		const int32_t secondsInJul{ 31 * 24 * 60 * 60 };
		const int32_t secondsInAug{ 31 * 24 * 60 * 60 };
		const int32_t secondsInSep{ 30 * 24 * 60 * 60 };
		const int32_t secondsInOct{ 31 * 24 * 60 * 60 };
		const int32_t secondsInNov{ 30 * 24 * 60 * 60 };
		const int32_t secondsInDec{ 31 * 24 * 60 * 60 };
		const int32_t secondsPerMinute{ 60 };
		const int32_t secondsPerHour{ 60 * 60 };
		const int32_t secondsPerDay{ 60 * 60 * 24 };
	};

	template<typename ObjectType> bool waitForTimeToPass(UnboundedMessageBlock<ObjectType>& outBuffer, ObjectType& argOne, int32_t timeInMsNew) {
		StopWatch stopWatch{ std::chrono::milliseconds{ timeInMsNew } };
		bool didTimePass{ false };
		while (!outBuffer.tryReceive(argOne)) {
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
			if (stopWatch.hasTimePassed()) {
				didTimePass = true;
				break;
			}
		};
		return didTimePass;
	}

	template<typename ObjectType> bool waitForTimeToPass(TSUnboundedMessageBlock<ObjectType>& outBuffer, ObjectType& argOne, int32_t timeInMsNew) {
		StopWatch stopWatch{ std::chrono::milliseconds{ timeInMsNew } };
		bool didTimePass{ false };
		while (!outBuffer.tryReceive(argOne)) {
			std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
			if (stopWatch.hasTimePassed()) {
				didTimePass = true;
				break;
			}
		};
		return didTimePass;
	}

	/**
	 * \addtogroup utilities
	 * @{
	 */

	/// Time formatting methods. \brief Time formatting methods.
	enum class TimeFormat {
		LongDate = 'D',///< "20 April 2021" - Long Date
		LongDateTime = 'F',///< "Tuesday, 20 April 2021 16:20" - Long Date/Time
		LongTime = 'T',///< "16:20:30" - Long Time
		ShortDate = 'd',///< "20/04/2021" - Short Date
		ShortDateTime = 'f',///< "20 April 2021 16:20" - Short Date/Time
		ShortTime = 't',///< "16:20" - Short Time
	};

	/**@}*/

	std::string getISO8601TimeStamp(const std::string& year, const std::string& month, const std::string& day, const std::string& hour, const std::string& minute,
		const std::string& second);

	void reportException(const std::string& stackTrace, UnboundedMessageBlock<std::exception>* sendBuffer = nullptr, bool rethrow = false);

	std::string convertTimeInMsToDateTimeString(uint64_t timeInMs, TimeFormat timeFormat);

	std::string convertToLowerCase(const std::string& stringToConvert);

	uint64_t convertTimestampToMsInteger(const std::string& timeStamp);

	std::string base64Encode(const std::string&, bool = false);

	std::string convertMsToDurationString(int32_t durationInMs);

	std::string loadFileContents(const std::string& filePath);

	std::string utf8MakeValid(const std::string& inputString);

	std::string urlEncode(const std::string& inputString);

	std::string urlDecode(const std::string& inputString);

	void spinLock(uint64_t timeInNsToSpinLockFor);

	std::string getCurrentISO8601TimeStamp();

	std::string generateBase64EncodedKey();

	std::string shiftToBrightGreen();

	std::string shiftToBrightBlue();

	std::string shiftToBrightRed();

	bool nanoSleep(int64_t ns);

	std::string reset();

	/**
	 * \addtogroup foundation_entities
	 * @{
	 */

	/// Permission values, for a given Channel, by Role or GuildMember. \brief Permission values, for a given Channel, by Role or GuildMember.
	enum class Permission : uint64_t {
		Create_Instant_Invite = 1ull << 0,///< Create Instant Invite.
		Kick_Members = 1ull << 1ull,///< Kick Members.
		Ban_Members = 1ull << 2,///< Ban Members.
		Administrator = 1ull << 3,///< Administrator.
		Manage_Channels = 1ull << 4,///< Manage Channels.
		Manage_Guild = 1ull << 5,///< Manage Guild.
		Add_Reactions = 1ull << 6,///< Add Reactions.
		View_Audit_Log = 1ull << 7,///< View Audit Log.
		Priority_Speaker = 1ull << 8,///< Priority Speaker.
		Stream = 1ull << 9,///< Stream.
		View_Channel = 1ull << 10,///< View Channel.
		Send_Messages = 1ull << 11,///< Send Messages.
		Send_Tts_Messages = 1ull << 12,///< Send TTS Messages.
		Manage_Messages = 1ull << 13,///< Manage Messages.
		Embed_Links = 1ull << 14,///< Embed Links.
		Attach_Files = 1ull << 15,///< Attach Files.
		Read_Message_History = 1ull << 16,///< Read Message History.
		Mention_Everyone = 1ull << 17,///< Mention Everyone.
		Use_External_Emojis = 1ull << 18,///< Use External Emoji.
		View_Guild_Insights = 1ull << 19,///< View Guild Insights.
		Connect = 1ull << 20,///< Connect.
		Speak = 1ull << 21,///< Speak.
		Mute_Members = 1ull << 22,///< Mute Members.
		Deafen_Members = 1ull << 23,///< Deafen Members.
		Move_Members = 1ull << 24,///< Move Members.
		Use_Vad = 1ull << 25,///< Use VAD.
		Change_Nickname = 1ull << 26,///< Change Nickname.
		Manage_Nicknames = 1ull << 27,///< Manage Nicknames.
		Manage_Roles = 1ull << 28,///< Manage Roles.
		Manage_Webhooks = 1ull << 29,///< Manage Webhooks.
		Manage_Emojis_And_Stickers = 1ull << 30,///< Manage Emojis And Stickers.
		Use_Application_Commands = 1ull << 31,///< Use Application Commands.
		Request_To_Speak = 1ull << 32,///< Request To Speak.
		Manage_Events = 1ull << 33,///< Manage Events.
		Manage_Threads = 1ull << 34,///< Manage Threads.
		Create_Public_Threads = 1ull << 35,///< Create Public Threads.
		Create_Private_Threads = 1ull << 36,///< Create Private Threads.
		Use_External_Stickers = 1ull << 37,///< Use External Stickers.
		Send_Messages_In_Threads = 1ull << 38,///< Send Messages In Threads.
		Start_Embedded_Activities = 1ull << 39,///< Start Embedded Activities.
		Moderate_Members = 1ull << 40///< Moderate Members.
	};

	/**@}*/

	std::ostream& operator<<(std::ostream& outputSttream, const std::string& (*theFunction)( void ));

	/**
	 * \addtogroup utilities
	 * @{
	 */

	template<typename StoredAsType, typename FlagType> StoredAsType setbool(StoredAsType inputFlag, FlagType theFlag, bool enabled) {
		if (enabled) {
			inputFlag |= static_cast<StoredAsType>(theFlag);
			return inputFlag;
		} else {
			inputFlag &= ~static_cast<StoredAsType>(theFlag);
			return inputFlag;
		}
	}

	template<typename StoredAsType, typename FlagType> bool getbool(StoredAsType inputFlag, FlagType theFlag) {
		return static_cast<StoredAsType>(inputFlag) & static_cast<StoredAsType>(theFlag);
	}

	/// Deduces whether or not a chosen period of time has passed, for a chosen timestamp. \brief Deduces whether or not a chosen period of time has passed, for a chosen timestamp.
	/// \param timeStamp A std::string representing the timestamp that you would like to check for time-elapsement.
	/// \param days An uint64_t representing the number of days to check for.
	/// \param hours An uint64_t representing the number of hours to check for.
	/// \param minutes An uint64_t representing the number of minutes to check for.
	/// \returns bool A bool denoting whether or not the input period of time has elapsed since the supplied timestamp.
	bool hasTimeElapsed(const std::string& timeStamp, uint64_t days = 0, uint64_t hours = 0, uint64_t minutes = 0);

	/// Collects a timestamp that is a chosen number of minutes ahead of the current time. \brief Collects a timestamp that is a chosen number of minutes ahead of the current time.
	/// \param minutesToAdd An int32_t containing the number of minutes to increment the timestamp forward for.
	/// \param hoursToAdd An int32_t containing the number of hours to increment the timestamp forward for.
	/// \param daysToAdd An int32_t containing the number of days to increment the timestamp forward for.
	/// \param monthsToAdd An int32_t containing the number of months to increment the timestamp forward for.
	/// \param yearsToAdd An int32_t containing the number of years to increment the timestamp forward for.
	/// \returns std::string A string containing the new ISO8601 timestamp.
	std::string getFutureISO8601TimeStamp(int32_t minutesToAdd, int32_t hoursToAdd = 0, int32_t daysToAdd = 0, int32_t monthsToAdd = 0, int32_t yearsToAdd = 0);

	/// Acquires a timestamp with the current time and date - suitable for use in message-embeds. \brief Acquires a timestamp with the current time and date - suitable for use in message-embeds.
	/// \returns std::string A std::string containing the current date-time stamp.
	std::string getTimeAndDate();

	struct WebSocketMessageReal {
		WebSocketMessageReal() noexcept = default;
		WebSocketMessageReal(simdjson::ondemand::value);
		uint64_t op{ static_cast<uint64_t>(-1) };
		template<typename ReturnType> ReturnType processJsonMessage() {
			return ReturnType{ this->d };
		}
		simdjson::ondemand::value d{};
		std::string t{};
		uint64_t s{};
	};

	/// Class for representing a timestamp. \brief Class for representing a timestamp.
	class TimeStamp {
	  public:
		operator std::string() {
			return this->originalTimeStamp;
		}

		TimeStamp& operator=(std::string&& originalTimeStampNew) {
			this->originalTimeStamp = originalTimeStampNew;
			return *this;
		}

		TimeStamp(std::string&& originalTimeStampNew) {
			*this = originalTimeStampNew;
		}

		TimeStamp& operator=(std::string& originalTimeStampNew) {
			this->originalTimeStamp = originalTimeStampNew;
			return *this;
		}

		TimeStamp(std::string& originalTimeStampNew) {
			*this = originalTimeStampNew;
		}

		/// Collects a timestamp using the format TimeFormat, as a string. \brief Collects a timestamp using the format TimeFormat, as a string.
		/// \param timeFormat A TimeFormat value, for selecting the output type.
		/// \returns string A string containing the returned timestamp.
		std::string getDateTimeStamp(TimeFormat timeFormat) {
			this->timeStampInMs = convertTimestampToMsInteger(this->originalTimeStamp);
			std::string newString = convertTimeInMsToDateTimeString(this->timeStampInMs, timeFormat);
			return newString;
		}

		/// Returns the original timestamp, from a Discord entity. \brief Returns the original timestamp, from a Discord entity.
		/// \returns string A string containing the returned timestamp.
		std::string getOriginalTimeStamp() {
			return this->originalTimeStamp;
		}

	  protected:
		std::string originalTimeStamp{};
		uint64_t timeStampInMs{ 0 };
	};

	/**@}*/

	/**
	 * \addtogroup foundation_entities
	 * @{
	 */

	/// Timeout durations for the timeout command. \brief Timeout durations for the timeout command.
	enum class TimeoutDurations {
		None = 0,///< None - remove timeout.
		Minute = 1,///< 1 Minute timeout.
		Five_Minutes = 5,///< 5 Minute timeout.
		Ten_Minutes = 10,///< 10 Minute timeout.
		Hour = 60,///< 1 Hour timeout.
		Day = 1440,///< 1 Day timeout.
		Week = 10080///< 1 Week timeout.
	};

	/// Gateway intents. \brief Gateway intents.
	enum class GatewayIntents : int32_t {
		Guilds = 1 << 0,///< Intent for receipt of Guild information.
		Guild_Members = 1 << 1,///< Intent for receipt of Guild members.
		Guild_Bans = 1 << 2,///< Intent for receipt of Guild bans.
		Guild_Emojis = 1 << 3,///< Intent for receipt of Guild emojis.
		Guild_Integrations = 1 << 4,///< Intent for receipt of Guild integrations.
		Guild_Webhooks = 1 << 5,///< Intent for receipt of Guild webhooks.
		Guild_Invites = 1 << 6,///< Intent for receipt of Guild invites.
		Guild_VoiceStates = 1 << 7,///< Intent for receipt of Guild voice states.
		Guild_Presences = 1 << 8,///< Intent for receipt of Guild presences.
		Guild_Messages = 1 << 9,///< Intent for receipt of Guild messages.
		Guild_Message_Reactions = 1 << 10,///< Intent for receipt of Guild message reactions.
		Guild_Message_Typing = 1 << 11,///< Intent for receipt of Guild message typing notifications.
		Direct_Messages = 1 << 12,///< Intent for receipt of direct messages (DMs).
		Direct_Message_Reactions = 1 << 13,///< Intent for receipt of direct message reactions.
		Direct_Message_Typing = 1 << 14,///< Intent for receipt of direct message typing notifications.
		Message_Content = 1 << 15,///< Intent for receipt of message content.
		Guild_Scheduled_Events = 1 << 16,///< Scheduled events.
		Default_Intents = Guilds | Guild_Bans | Guild_Emojis | Guild_Integrations | Guild_Webhooks | Guild_Invites | Guild_VoiceStates | Guild_Messages | Guild_Message_Reactions |
			Guild_Message_Typing | Direct_Messages | Direct_Message_Reactions | Direct_Message_Typing | Guild_Scheduled_Events,///< Default intents (all non-privileged intents).
		Privileged_Intents = Guild_Members | Guild_Presences | Message_Content,///< Privileged intents requiring ID.
		All_Intents = Default_Intents | Privileged_Intents///< Every single intent.
	};

	/// For ids of DiscordEntities. \brief For ids of DiscordEntities.
	using Snowflake = std::string;

	/// Base class  for all Discord entities. \brief Base class  for all Discord entities.
	class DiscordEntity {
	  public:
		Snowflake id{};///< The identifier "snowflake" of the given entity.
		/// Converts the snowflake-id into a time and date stamp. \brief Converts the
		/// snowflake-id into a time and date stamp. \returns std::string A
		/// std::string containing the timestamp.
		std::string getCreatedAtTimestamp(TimeFormat timeFormat);

		virtual ~DiscordEntity() = default;
	};

	/// Role tags data. \brief Role tags data.
	struct RoleTagsData {
		bool premiumSubscriber{};///< Are they a premium subscriber?
		std::string integrationId{};///< What is the integration id?
		std::string botId{};///< What is the bot id?
	};

	enum class RoleFlags : int8_t { Mentionable = 1 << 0, Managed = 1 << 1, Hoist = 1 << 2 };

	/// Data structure representing a single Role. \brief Data structure representing a single Role.
	class RoleData : public DiscordEntity {
	  public:
		std::string unicodeEmoji{};///< Emoji representing the Role.
		std::string permissions{};
		int32_t position{ 0 };///< Its position amongst the rest of the Guild's roles.
		RoleTagsData tags{};///< Role tags for the Role.
		bool hoist{ false };///< Is it hoisted?
		std::string name{};///< The Role's name.
		int32_t color{ 0 };///< The Role's color.
		int8_t flags{ 0 };///< Role flags.
		std::string icon{};///< Icon representing the Role.
		bool mentionable{};
		bool managed{};

		virtual ~RoleData() = default;
	};

	/// User flags. \brief User flags.
	struct UnavailableGuild : public DiscordEntity {
		bool unavailable{ true };
	};

	/// User flags. \brief User flags.
	enum class UserFlags : int32_t {
		Staff = 1 << 0,///< Discord Employee.
		Partner = 1 << 1,///< Partnered Server Owner.
		Hypesquad = 1 << 2,///< HypeSquad Events Member.
		Bug_Hunter_Level_1 = 1 << 3,///< Bug Hunter Level 1.
		Hypesquad_Online_House_1 = 1 << 6,///< House Bravery Member.
		Hypesquad_Online_House_2 = 1 << 7,///< House Brilliance Member.
		Hypesquad_Online_House_3 = 1 << 8,///< House Balance Member.
		Premium_Early_Suppoerter = 1 << 9,///< Early Nitro Supporter.
		Team_Pseudo_User = 1 << 10,///< User is a team.
		Bug_Hunter_Level_2 = 1 << 14,///< Bug Hunter Level 2.
		Verified_Bot = 1 << 16,///< Verified Bot.
		Verified_Developer = 1 << 17,///< Early Verified Bot Developer.
		Certified_Moderator = 1 << 18,///< Discord Certified Moderator.
		Bot_Http_Interactions = 1 << 19,///< Bot uses only HTTP interactions and is shown in the online member list.
		Bot = 1 << 20,///< Is it a bot?
		MFAEnabled = 1 << 21,///< Is MFA enabled?
		System = 1 << 22,///< Is it a system integration?
		Verified = 1 << 23///< Is it verified?
	};

	/// Premium types denote the level of premium a user has. \brief Premium types denote the level of premium a user has.
	enum class PremiumType : int8_t {
		None = 0,///< None.
		Nitro_Classic = 1,///< Nitro classic.
		Nitro = 2///< Nitro.
	};

	/// Data structure representing a single User. \brief Data structure representing a single User.
	class UserData : public DiscordEntity {
	  public:
		UserData() = default;

		std::string discriminator{};///< The user's 4-digit discord-tag	identify.
		PremiumType premiumType{};///< The type of Nitro subscription on a user's account.
		int32_t accentColor{ 0 };///< The user 's banner color encoded as an integer representation of hexadecimal color code.
		std::string userName{};///< The user's userName, not unique across the platform	identify.
		int32_t publicFlags{};///< The public flags on a user' s account.
		std::string avatar{};///< The user's avatar hash.
		std::string banner{};///< The user's banner hash.
		std::string locale{};///< The user' s chosen language option.
		std::string email{};///< The user's email.
		bool mfaEnabled{};
		int32_t flags{};///< The public flags on a user' s account.
		bool verified{};
		bool system{};
		bool bot{};

		virtual ~UserData() = default;
	};

	/// Attachment data. \brief Attachment data.
	class AttachmentData : public DiscordEntity {
	  public:
		std::string contentType{};///< Type of content for the attachment.
		std::string description{};///< A description of the attachment.
		bool ephemeral{ false };///< Whether it was an ephemeral response.
		std::string filename{};///< The file name of the attachment.
		std::string proxyUrl{};///< The proxy url for the attachment.
		int32_t height{ 0 };///< The height of the attachment.
		int32_t width{ 0 };///< The width of the attachment.
		int32_t size{ 0 };///< The size of the attachment.
		std::string url{};///< The url for the attachment.

		virtual ~AttachmentData() = default;
	};

	/// Sticker format types. \brief Sticker format types.
	enum class StickerFormatType {
		Png = 1,///< Png.
		Apng = 2,///< Apng.
		Lottie = 3///< Lottie
	};

	/// Embed footer data. \brief Embed footer data.
	struct EmbedFooterData {
		std::string proxyIconUrl{};///< Proxy icon url.
		std::string iconUrl{};///< Icon url.
		std::string text{};///< Footer text.
	};

	/// Embed image data. \brief Embed image data.
	struct EmbedImageData {
		std::string proxyUrl{};///< Proxy url.
		int32_t height{ 0 };///< Image height.
		int32_t width{ 0 };///< Image width.
		std::string url{};///< Image url.
	};

	/// Embed thumbnail data. \brief Embed thumbnail data.
	struct EmbedThumbnailData {
		std::string proxyUrl{};///< Proxy url.
		int32_t height{ 0 };///< Image height.
		int32_t width{ 0 };///< Image width.
		std::string url{};///< Image url.
	};

	/// Embed video data. \brief Embed video data.
	struct EmbedVideoData {
		std::string proxyUrl{};///< Proxy url.
		int32_t height{ 0 };///< Image height.
		int32_t width{ 0 };///< Image width.
		std::string url{};///< Image url.
	};

	/// Embed provider data. \brief Embed provider data.
	struct EmbedProviderData {
		std::string name{};///< Name.
		std::string url{};///< Url.
	};

	/// Embed author data. \brief Embed author data.
	struct EmbedAuthorData {
		std::string proxyIconUrl{};///< Proxy icon url.
		std::string iconUrl{};///< Icon url.
		std::string name{};///< Name.
		std::string url{};///< Url.
	};

	/// Embed field data. \brief Embed field data.
	struct EmbedFieldData {
		bool Inline{ false };///< Is the field inline with the rest of them?
		std::string value{};///< The text on the field.
		std::string name{};///< The title of the field.
	};

	/// Embed types. \brief Embed types.
	enum class EmbedType {
		Rich = 0,///< Rich.
		Image = 1,///< Image.
		Video = 2,///< Video.
		Gifv = 3,///< Gifv.
		Article = 4,///< Article.
		link = 5///< Link.
	};

	/// Embed data. \brief Embed data.
	class EmbedData {
	  public:
		std::string hexColorValue{ "000000" };///< Hex color value of the embed.
		std::vector<EmbedFieldData> fields{};///< Array of embed fields.
		EmbedThumbnailData thumbnail{};///< Embed thumbnail data.
		EmbedProviderData provider{};///< Embed provider data.
		std::string description{};///< Description of the embed.
		EmbedFooterData footer{};///< Embed footer data.
		EmbedAuthorData author{};///< Embed author data.
		std::string timestamp{};///< Timestamp to be placed on the embed.
		EmbedImageData image{};///< Embed image data.
		EmbedVideoData video{};///< Embed video data.
		std::string title{};///< Title of the embed.
		std::string type{};///< Type of the embed.
		std::string url{};///< Url for the embed.

		/// Sets the author's name and avatar for the embed. \brief Sets the author's name and avatar for the embed.
		/// \param authorName The author's name.
		/// \param authorAvatarUrl The url to their avatar.
		/// \returns EmbedData& A reference to this embed.
		EmbedData& setAuthor(const std::string& authorName, const std::string& authorAvatarUrl = "") {
			this->author.name = authorName;
			this->author.iconUrl = authorAvatarUrl;
			return *this;
		}

		/// Sets the footer's values for the embed. \brief Sets the footer's values for the embed.
		/// \param footerText The footer's text.
		/// \param footerIconUrlText Url to the footer's icon.
		/// \returns EmbedData& A reference to this embed.
		EmbedData& setFooter(const std::string& footerText, const std::string& footerIconUrlText = "") {
			this->footer.text = footerText;
			this->footer.iconUrl = footerIconUrlText;
			return *this;
		}

		/// Sets the timestamp on the embed. \brief Sets the timestamp on the embed.
		/// \param timeStamp The timestamp to be set.
		/// \returns EmbedData& A reference to this embed.
		EmbedData& setTimeStamp(const std::string& timeStamp) {
			this->timestamp = timeStamp;
			return *this;
		}

		/// Adds a field to the embed. \brief Adds a field to the embed.
		/// \param name The title of the embed field.
		/// \param value The contents of the embed field.
		/// \param Inline Is it inline with the rest of the fields on the embed?
		/// \returns EmbedData& A reference to this embed.
		EmbedData& addField(const std::string& name, const std::string& value, bool Inline = true) {
			this->fields.emplace_back(EmbedFieldData{ .Inline = Inline, .value = value, .name = name });
			return *this;
		}

		/// Sets the description (the main contents) of the embed. \brief Sets the description (the main contents) of the embed.
		/// \param descriptionNew The contents of the description to set.
		/// \returns EmbedData& A reference to this embed.
		EmbedData& setDescription(const std::string& descriptionNew) {
			this->description = descriptionNew;
			return *this;
		}

		/// Sets the color of the embed, by applying a hex-color value. \brief Sets the color of the embed, by applying a hex-color value.
		/// \param hexColorValueNew A std::string containing a hex-number value (Between 0x00 0xFFFFFF).
		/// \returns EmbedData& A reference to this embed.
		EmbedData& setColor(const std::string& hexColorValueNew) {
			this->hexColorValue = hexColorValueNew;
			return *this;
		}

		/// Sets the thumbnail of the embed. \brief Sets the thumbnail of the embed.
		/// \param thumbnailUrl The url to the thumbnail to be used.
		/// \returns EmbedData& A reference to this embed.
		EmbedData& setThumbnail(const std::string& thumbnailUrl) {
			this->thumbnail.url = thumbnailUrl;
			return *this;
		}

		/// Sets the title of the embed. \brief Sets the title of the embed.
		/// \param titleNew A std::string containing the desired title.
		/// \returns EmbedData& A reference to this embed.
		EmbedData& setTitle(const std::string& titleNew) {
			this->title = titleNew;
			return *this;
		}

		/// Sets the image of the embed. \brief Sets the image of the embed.
		/// \param imageUrl The url of the image to be set on the embed.
		/// \returns EmbedData& A reference to this embed.
		EmbedData& setImage(const std::string& imageUrl) {
			this->image.url = imageUrl;
			return *this;
		}
	};

	/// Message reference data.\brief Message reference data.
	struct MessageReferenceData {
		bool failIfNotExists{ false };///< Fail if the Message doesn't exist?
		std::string messageId{};///< Id of the Message to reference.
		std::string channelId{};///< Id of the Channel that the referenced Message was sent in.
		std::string guildId{};///< Id of the Guild that the referenced Message was sent in.
	};

	enum class MediaType { png = 0, gif = 1, jpeg = 2, mpeg = 3, mp3 = 4 };

	/// Data representing a file to be sent via multipart-form data. \brief Data representing a file to be sent via multipart-form data.
	struct File {
		std::string fileName{};///< The name of the file.
		std::string data{};///< The data of the file.
	};

	/// Permission overwrites types. \brief Permission overwrites types.
	enum class PermissionOverwritesType {
		Role = 0,///< Role.
		User = 1///< User.
	};

	/// A Permission overwrite, for a given Channel. \brief A Permission overwrite, for a given Channel.
	class OverWriteData : public DiscordEntity {
	  public:
		PermissionOverwritesType type{};///< Role or User type.
		std::string channelId{};///< Channel id for which Channel this overwrite belongs to.

		virtual ~OverWriteData() = default;
	};

	/// Channel types. \brief Channel types.
	enum class ChannelType : int8_t {
		Guild_Text = 0,///< Guild text.
		Dm = 1,///< Direct-Message.
		Guild_Voice = 2,/// Guild voice.
		Group_Dm = 3,///< Group direct-Message.
		Guild_Category = 4,///< Guild category.
		Guild_News = 5,///< Guild news.
		Guild_Store = 6,///< Guild store.
		Guild_News_Thread = 10,///< Guild news Thread.
		Guild_Public_Thread = 11,///< Guild public Thread.
		Guild_Private_Thread = 12,///< Guild private Thread.
		Guild_Stage_Voice = 13,///< Guild stage-voice.
		Guild_Directory = 14,///< The channel in a hub containing the listed servers.
		Guild_Forum = 15///< A channel that can only contain threads.
	};

	/// Meta data for a Thread type of Channel. \brief Meta data for a Thread type of Channel.
	struct ThreadMetadataData {
		int32_t autoArchiveDuration{ 0 };///< How uint64_t before archiving this Thread.
		TimeStamp archiveTimestamp{ "" };///< (Where applicable) the time at which this Thread was archived.
		bool invitable{ false };///< The id of the individual who archived this Thread.
		bool archived{ false };///< Whether or not this Thread is currently archived.
		bool locked{ false };///< Whether or not this Thread is currently locked.
	};

	/// Data for a single member of a Thread. \brief Data for a single member of a Thread.
	class ThreadMemberData : public DiscordEntity {
	  public:
		TimeStamp joinTimestamp{ "" };///< The time at which the member joined this Thread.
		int32_t flags{ 0 };///< Flags.
		std::string userId{};///< The User's id.

		virtual ~ThreadMemberData() = default;
	};

	/// Thread types. \brief Thread types.
	enum class ThreadType {
		Guild_News_Thread = 10,///< Guild news Thread.
		Guild_Public_Thread = 11,///< Guild public Thread.
		Guild_Private_Thread = 12///< Guild private Thread.
	};

	/// Automatic Thread archiving durations. \brief Automatic Thread archiving durations.
	enum class ThreadAutoArchiveDuration : int32_t {
		Shortest = 60,///< Shortest.
		Short = 1440,///< Short.
		Long = 4320,///< Long.
		Longest = 10080///< Longest.
	};

	enum class ChannelFlags : int8_t { NSFW = 1 << 0 };

	/// Data structure representing a single Channel. \brief Data structure representing a single Channel.
	class ChannelData : public DiscordEntity {
	  public:
		std::unordered_map<std::string, OverWriteData> permissionOverwrites{};///< Permission overwrites for the given Channel.
		std::unordered_map<std::string, UserData> recipients{};///< Recipients, in the case of a group Dm or Dm.
		int32_t defaultAutoArchiveDuration{ 0 };///< Default time it takes to archive a thread.
		ThreadMetadataData threadMetadata{};///< Metadata in the case that this Channel is a Thread.
		ChannelType type{ ChannelType::Dm };///< The type of the Channel.
		TimeStamp lastPinTimestamp{ "" };///< Timestamp of the last pinned Message.
		int32_t videoQualityMode{ 0 };///< Video quality mode.
		int32_t rateLimitPerUser{ 0 };///< Amount of seconds a User has to wait before sending another Message.
		std::string lastMessageId{};///< Id of the last Message.
		ThreadMemberData member{};///< Thread member object for the current User, if they have joined the Thread.
		int32_t messageCount{ 0 };///< An approximate count of Messages in a Thread stops counting at 50.
		std::string permissions{};///< Computed permissions for the invoking user in the channel, including overwrites.
		std::string applicationId{};///< Application id of the current application.
		int32_t memberCount{ 0 };///< Count of members active in the Channel.
		std::string rtcRegion{};///< Real-time clock region.
		std::string parentId{};///< Id of the Channel's parent Channel/category.
		int32_t userLimit{ 0 };///< User limit, in the case of voice channels.
		std::string ownerId{};///< Id of the Channel's owner.
		int32_t position{ 0 };///< The position of the Channel, in the Guild's Channel list.
		std::string guildId{};///< Id of the Channel's Guild, if applicable.
		int32_t bitrate{ 0 };///< Bitrate of the Channel, if it is a voice Channel.
		std::string topic{};///< The Channel's topic.
		int8_t flags{ 0 };///< Channel flags combined as a bitfield.
		std::string icon{};///< Icon for the Channel, if applicable.
		std::string name{};///< Name of the Channel.

		virtual ~ChannelData() = default;
	};

	enum class GuildMemberFlags : int8_t { Pending = 1 << 0, Deaf = 1 << 1, Mute = 1 << 2 };

	/// Data structure representing a single GuildMember. \brief Data structure representing a single GuildMember.
	class GuildMemberData : public DiscordEntity {
	  public:
		TimeStamp communicationDisabledUntil{ "" };///< When the user's timeout will expire and the user will be able to communicate in the guild again.
		std::vector<std::string> roles{};///< The Guild roles that they have.
		std::string premiumSince{};///< If applicable, when they first boosted the server.
		std::string permissions{};
		TimeStamp joinedAt{ "" };///< When they joined the Guild.
		std::string userAvatar{};///< This GuildMember's User Avatar.
		std::string userName{};///< This GuildMember's UserName.
		std::string guildId{};///< The current Guild's id.
		std::string avatar{};///< The member's guild avatar hash.
		std::string nick{};///< Their nick/display name.
		int8_t flags{ 0 };///< GuildMember flags.
		UserData user{};
		bool pending{};
		bool mute{};
		bool deaf{};

		virtual ~GuildMemberData() = default;
	};

	/// Voice state data. \brief Voice state data.
	struct VoiceStateData {
		TimeStamp requestToSpeakTimestamp{ "" };///< The time at which the User requested to speak.
		bool selfStream{ false };///< Whether this User is streaming using "Go Live".
		GuildMemberData member{};///< The Guild member id this voice state is for.
		std::string sessionId{};///< The session id for this voice state.
		bool selfVideo{ false };///< Whether this User's camera is enabled.
		bool selfDeaf{ false };///< Whether this User is locally deafened.
		bool selfMute{ false };///< Whether this User is locally muted.
		bool suppress{ false };///< Whether this User is muted by the current User.
		std::string channelId{};///< The Channel id this User is connected to.
		bool deaf{ false };///< Whether this User is deafened by the server.
		bool mute{ false };///< Whether this User is muted by the server.
		std::string guildId{};///< The Guild id this voice state is for.
		std::string userId{};///< The User id this voice state is for.
	};

	/// Data representing an active Thread. \brief Data representing an active Thread.
	struct ActiveThreadsData {
		std::vector<ThreadMemberData> members{};
		std::vector<ChannelData> threads{};
		bool hasMore{ false };
	};

	/// Data representing an archived Thread. \brief Data representing an archived Thread.
	struct ArchivedThreadsData : public ActiveThreadsData {};

	/// Application command-option types. \brief Application command-option types.
	enum class ApplicationCommandOptionType {
		Sub_Command = 1,///< Sub-command.
		Sub_Command_Group = 2,///< Sub-command group.
		String = 3,///< std::string.
		Integer = 4,///< Integer.
		boolean = 5,///< boolean.
		User = 6,///< User.
		Channel = 7,///< Channel.
		Role = 8,///< Role.
		Mentionable = 9,///< Mentionable.
		Number = 10,///< Number.
		Attachment = 11///< Attachment.
	};

	/// Application command permission-types. \brief Application command permission-types.
	enum class ApplicationCommandPermissionType {
		Role = 1,///< Role.
		User = 2,///< User.
		Channel = 3///< Channel.
	};

	/// Permissions data for an ApplicationCommand. \brief Permissions data for an ApplicationCommand.
	class ApplicationCommandPermissionData : public DiscordEntity {
	  public:
		ApplicationCommandPermissionType type{ ApplicationCommandPermissionType::Role };///< The type of Permission.
		bool permission{ false };///< Whether the Permission is active or not.

		virtual ~ApplicationCommandPermissionData() = default;
	};

	/// Represents the Permissions for accessing an ApplicationCommand from within a Guild. \brief Represents the Permissions for accessing an ApplicationCommand from within a Guild.
	class GuildApplicationCommandPermissionsData : public DiscordEntity {
	  public:
		std::vector<ApplicationCommandPermissionData> permissions{};///< The Permissions.
		std::string applicationId{};///< The application's id.
		std::string guildId{};///< The Guild's id.

		virtual ~GuildApplicationCommandPermissionsData() = default;
	};

	/// Data structure representing a single emoji. \brief Data structure representing a single emoji.
	class EmojiData : public DiscordEntity {
	  public:
		std::wstring unicodeName{ L"" };///< What is its unicode name?
		std::vector<RoleData> roles{};///< Roles that are allowed to use this emoji.
		bool requireColons{ false };///< Require colons to render it?
		bool available{ true };///< Is it available to be used?
		bool animated{ false };///< Is it animated?
		bool managed{ false };///< Is it managed?
		std::string name{};///< What is its name?
		UserData user{};///< User that created this emoji.

		virtual ~EmojiData() = default;
	};

	/// For updating/modifying a given Channel's properties. \brief For updating/modifying a given Channel's properties.
	struct UpdateChannelData {
		std::unordered_map<uint64_t, OverWriteData> permissionOverwrites{};
		int32_t defaultAutoArchiveDuration{ 10080 };
		int32_t videoQualityMode{ 1 };
		int32_t rateLimitPerUser{ 0 };
		int32_t bitrate{ 48000 };
		std::string parentId{};
		std::string rtcRgion{};
		int32_t userLimit{ 0 };
		int32_t position{ 0 };
		std::string topic{};
		std::string name{};
		ChannelType type{};
		bool nsfw{ false };
	};

	/// Data structure representing a single reaction. \brief/// Data structure representing a single reaction.
	class ReactionData : public DiscordEntity {
	  public:
		GuildMemberData member{};///< The GuildMember who placed the reaction.
		std::string channelId{};///< The id of the Channel where it was placed.
		std::string messageId{};///< The id of the Message upon which it was placed.
		std::string guildId{};///< The id of the Guild where it was placed.
		int32_t count{ 0 };///< The number of times this particular emoji was placed as a reaction to the given Message.
		EmojiData emoji{};///< The emoji that was placed as a reaction.
		uint64_t userId{};///< The id of the User who placed the reaction.
		bool me{ false };///< Whether or not I (The bot) placed it.

		virtual ~ReactionData() = default;
	};

	/// Structure representing Voice Region Data. \brief Structure representing Voice Region Data.
	struct VoiceRegionData {
		bool deprecated{ false };///< Whether this is a deprecated voice region(avoid switching to these).
		bool optimal{ false };///< True for a single server that is closest to the current User's client.
		bool custom{ false };///< Whether this is a custom voice region(used for events / etc).
		std::string name{};///< Name of the region.
		std::string id{};///< Unique ID for the region.
	};

	/// Message activity types. \brief Message activity types.
	enum class MessageActivityType {
		Join = 1,///< Join.
		Spectate = 2,///< Spectate.
		Listen = 3,///< Listen.
		Join_Request = 5///< Join-request.
	};

	/// Message activity data. \brief Message activity data.
	struct MessageActivityData {
		MessageActivityType type{ MessageActivityType::Join };///< Message activity type.
		std::string partyId{};///< Party id.
	};

	/// Ban data. \brief Ban data.
	struct BanData {
		bool failedDueToPerms{ false };///< Failed due to perms?
		std::string reason{};///< Reason for the ban.
		UserData user{};///< User that was banned.
	};

	/// Team members object data. \brief Team members object data.
	struct TeamMembersObjectData {
		int32_t membershipState{ 0 };///< Current state.
		std::string teamId{};///< Id of the current team.
		UserData user{};///< User data of the current User.
	};

	/// For updating the current voice state. \brief For updating the current voice state.
	struct UpdateVoiceStateData {
		bool selfMute{ false };///< Whether or not we self-mute ourselves.
		bool selfDeaf{ false };///< Whether or not we self-deafen ourselves.
		std::string channelId{};///< Id of the desired voice Channel. Leave blank to disconnect.
		std::string guildId{};///< The id of the Guild fo which we would like to establish a voice connection.
	};

	/// Team object data. \brief Team object data.
	class TeamObjectData : public DiscordEntity {
	  public:
		std::vector<TeamMembersObjectData> members{};///< Array of team members object data.
		std::string ownerUserId{};///< User id of the team owner.
		std::string icon{};///< Icon for the team.

		virtual ~TeamObjectData() = default;
	};

	/// Application flags, for the ApplicationData structure.
	enum class ApplicationFlags {
		Gateway_Presence = 1 << 12,///< Intent required for bots in 100 or more servers to receive presence_update events.
		Gateway_Presence_Limited = 1 << 13,///< Intent required for bots in under 100 servers to receive presence_update events, found in Bot Settings.
		Gateway_Guild_Members = 1 << 14,///< Intent required for bots in 100 or more servers to receive member-related events like guild_member_add.
		Gateway_Guild_Members_Limited = 1 << 15,///< Intent required for bots in under 100 servers to receive member-related events like guild_member_add, found in Bot Settings.
		Verificatino_Pending_Guild_Limit = 1 << 16,///< Indicates unusual growth of an app that prevents verification
		Embedded = 1 << 17,///< Indicates if an app is embedded within the Discord client (currently unavailable publicly)
		Gateway_Message_Content = 1 << 18,///< Intent required for bots in 100 or more servers to receive message content
		Gateway_Message_Content_Limited = 1 << 19///< Intent required for bots in under 100 servers to receive message content, found in Bot Settings};
	};

	/// Install params data, for application data. \brief Install params data, for application data.
	struct InstallParamsData {
		std::vector<std::string> scopes{};///< The scopes to add the application to the server with.
		std::string permissions{};///< The permissions to request for the bot role.
	};

	/// Application data. \brief Application data.
	class ApplicationData : public DiscordEntity {
	  public:
		std::vector<std::string> rpcOrigins{};///< Array of RPC origin strings.
		bool botRequireCodeGrant{ false };///< Does the bot require a code grant?
		std::vector<std::string> tags{};///< Up to 5 tags describing the content and functionality of the application install_params.
		std::string termsOfServiceUrl{};///< Terms of service Url.
		std::string privacyPolicyUrl{};///< Privacy policy Url.
		ApplicationFlags flags{ 0 };///< Application flags.
		InstallParamsData params{};///< Settings for the application's default in-app authorization link, if enabled std::string customInstallUrl{};
		std::string primarySkuId{};///< Primary SKU Id.
		std::string description{};///< Description of the application.
		std::string coverImage{};///< The cover image.
		bool botPublic{ false };///< Is the bot public?
		std::string verifyKey{};///< The verification key.
		std::string summary{};///< Summary of the application.
		TeamObjectData team{};///< Team object data.
		std::string guildId{};///< Guild id.
		std::string slug{};///< Sluhg.
		std::string name{};///< Application's name.
		std::string icon{};///< Application's icon.
		UserData owner{};///< Application's owner.

		virtual ~ApplicationData() = default;
	};

	/// Authorization info structure. \brief Authorization info structure.
	struct AuthorizationInfoData {
		std::vector<std::string> scopes{};///< Array of strings - the scopes the User has authorized the application for.
		ApplicationData application{};///< Partial application object the current application.
		std::string expires{};///< When the access token expires.
		UserData user{};/// The User who has authorized, if the User has authorized with the identify scope.
	};

	/// Account data. \brief Account data.
	class AccountData : public DiscordEntity {
	  public:
		std::string name{};///< Name of the account.
	};

	/// Guild Widget Data. \brief Guild Widget Data.
	struct GuildWidgetData {
		bool enabled{ false };///< Whether the widget is enabled.
		std::string channelId{};///< The widget Channel id.
	};

	/// Get Guild Widget Data. \brief Get Guild Widget Data.
	struct GetGuildWidgetObjectData : public DiscordEntity {
		std::vector<ChannelData> channels{};///< Voice and stage channels which are accessible by everyone.
		std::vector<UserData> members{};///< Special widget user objects that includes users presence (Limit 100).
		std::string instantInvite{};///< Instant invite for the guilds specified widget invite channel.
		int32_t presence_count{ 0 };///< Number of online members in this guild.
		std::string name{};///< Guild name (2-100 characters).
	};

	/// Widget style options. \brief Widget style options.
	enum class WidgetStyleOptions {
		Shield = 0,///< Shield
		Banner1 = 1,///< Banner1
		Banner2 = 2,///< Banner2
		Banner3 = 3,///< Banner3
		Banner4 = 4///< Banner4
	};

	/// Guild widget image data. \brief Guild widget image data.
	struct GuildWidgetImageData {
		std::string url{};
	};

	/// Integration data. \brief Integration data.
	class IntegrationData : public DiscordEntity {
	  public:
		int32_t expireGracePeriod{ 0 };///< How uint64_t before the integration expires.
		ApplicationData application{};///< Application data.
		int32_t subscriberCount{ 0 };///< Number of current subscribers.
		bool enableEmoticons{ true };///< Emoticons enabled?
		int32_t expireBehavior{ 0 };///< What to do upon expiry.
		TimeStamp syncedAt{ "" };///< Time it was last synced at.
		bool enabled{ false };///< Enabled?
		bool syncing{ false };///< Is it syncing?
		AccountData account{};///< Account data.
		bool revoked{ false };///< Has it been revoked?
		std::string name{};///< Name of the integration.
		std::string type{};///< Type of integration.
		uint64_t roleId{};///< Role Id.
		UserData user{};///< User data for the integration.

		virtual ~IntegrationData() = default;
	};

	/// Audit log events. \brief Audit log events.
	enum class AuditLogEvent {
		Guild_Update = 1,///< Guild update.
		Channel_Create = 10,///< Channel create.
		Channel_Update = 11,///< Channel update.
		Channel_Delete = 12,///< Channel delete.
		Channel_Overwrite_Create = 13,///< Channel overwrite create.
		Channel_Overwrite_Update = 14,///< Channel overwrite update.
		Channel_Overwrite_Delete = 15,///< Channel overwrite delete.
		Member_Kick = 20,///< Member kick.
		Member_Prune = 21,///< Member prune.
		Member_Ban_Add = 22,///< Member ban add.
		Member_Ban_Remove = 23,///< Member ban remove.
		Member_Update = 24,///< Member update.
		Member_Role_Update = 25,///< Member role update.
		Member_Move = 26,///< Member move.
		Member_Disconnect = 27,///< Member disconnect.
		Bot_Add = 28,///< Bot add.
		Role_Create = 30,///< Role create.
		Role_Update = 31,///< Role update.
		Role_Delete = 32,///< Role delete.
		Invite_Create = 40,///< Invite create.
		Invite_Update = 41,///< Invite update.
		Invite_Delete = 42,///< Invite delete.
		Webhook_Create = 50,///< Webhook create.
		Webhook_Update = 51,///< Webhook update.
		Webhook_Delete = 52,///< Webhook delete.
		Emoji_Create = 60,///< Emoji create.
		Emoji_Update = 61,///< Emoji update.
		Emoji_Delete = 62,///< Emoji delete.
		Message_Delete = 72,///< Message delete.
		Message_Bulk_Delete = 73,///< Message bulk delete.
		Message_Pin = 74,///< Message pin.
		Message_Unpin = 75,///< Message unpin.
		Integration_Create = 80,///< Integration create.
		Integration_Update = 81,///< Integration update.
		Integration_Delete = 82,///< Integration delete.
		Stage_Instance_Create = 83,///< Stage-Instance create.
		Stage_Instance_Update = 84,///< Stage-Instance update.
		Stage_Instance_Delete = 85,///< Stage-Instance delete.
		Sticker_Create = 90,///< Sticker create.
		Sticker_Update = 91,///< Sticker update.
		Sticker_Delete = 92,///< Sticker delete.
		Guild_Scheduled_Event_Create = 100,///< Guild-scheduled-event create.
		Guild_Scheduled_Event_Update = 101,///< Guild-scheduled-event update.
		Guild_Scheduled_Event_Delete = 102,///< Guild-scheduled-event delete.
		Thread_Create = 110,///< Thread create.
		Thread_Update = 111,///< Thread update.
		Thread_Delete = 112,///< Thread delete.
		Application_Command_Permission_Update = 121///< Permissions were updated for a command.
	};

	/// Audit log entry info data \brief Audit log entry info data.
	class OptionalAuditEntryInfoData : public DiscordEntity {
	  public:
		std::string deleteMemberDays{};///< Number of days for which the member's Messages were deleted.
		std::string membersRemoved{};///< Number of members that were removed upon a prune.
		std::string applicationId{};///< ID of the app whose permissions were targeted APPLICATION_COMMAND_PERMISSION_UPDATE.
		std::string roleName{};///< Role name.
		std::string channelId{};///< Channel Id.
		std::string messageId{};///< Message Id.
		std::string count{};///< Count.
		std::string type{};///< Type.

		virtual ~OptionalAuditEntryInfoData() = default;
	};

	/// Audit log change data. \brief Audit log change data.
	struct AuditLogChangeData {
		std::string newValue{};///< New value.
		std::string oldValue{};///< Old value.
		std::string key{};///< The key of the audit log change.
	};

	/// Guild prune count data. \brief Guild prune count data.
	struct GuildPruneCountData {
		int32_t count{ 0 };
	};

	/// Audit log entry data. \brief Audit log entry data.
	class AuditLogEntryData : public DiscordEntity {
	  public:
		std::vector<AuditLogChangeData> changes{};///< Array of audit log change data.
		OptionalAuditEntryInfoData options{};///< Audit log entry info data.
		std::string createdTimeStamp{};///< Time at which this entry was created.
		AuditLogEvent actionType{};///< Audit log action type.
		std::string reason{};///< The reason that was entered for the given change.
		uint64_t targetId{};///< Id of the target User.
		uint64_t userId{};///< Id of the executing User.

		virtual ~AuditLogEntryData() = default;
	};

	/// Party data. \brief Party data.
	class PartyData : public DiscordEntity {
	  public:
		std::vector<int32_t> size{ 0, 0 };///< The size of the party.

		virtual ~PartyData() = default;
	};

	/// Assets data. \brief Party data.
	struct AssetsData {
		std::string largeImage{};///< Keyname of an asset to display.
		std::string smallImage{};///< Keyname of an asset to display.
		std::string largeText{};///< Hover text for the large image.
		std::string smallText{};///< Hover text for the small image.
	};

	/// Secrets data. \brief Secrets data.
	struct SecretsData {
		std::string spectate{};///< Unique hash for the given match context.
		std::string match{};///< Unique hash for Spectate button.
		std::string join{};///< Unique hash for chat invitesand Ask to Join.
	};

	/// Timestamp data. \brief Timestamp data.
	struct TimestampData {
		uint64_t start{ 0 };///< Unix timestamp - Send this to have an "elapsed" timer.
		uint64_t end{ 0 };///< Unix timestamp - send this to have a "remaining" timer.
	};

	/// Button data. \brief Button data.
	struct ButtonData {
		std::string label{};///< Visible label of the button.
		std::string url{};///< Url to display on the button.
	};

	/// Activity types. \brief Activity types.
	enum class ActivityType {
		Game = 0,///< Game.
		Streaming = 1,///< Streaming.
		Listening = 2,///< Listening.
		Watching = 3,///< Watching.
		Custom = 4,///< Custom.
		Competing = 5///< Competing.
	};

	/// Activity data. \brief Activity data.
	struct ActivityData {
		std::string applicationId{};///< Application id for the current application.
		TimestampData timestamps{};///< Timestamp data.
		int32_t createdAt{ 0 };///< Timestamp of when the activity began.
		bool instance{ false };///< Whether this activity is an instanced context, like a match.
		SecretsData secrets{};///< Secrets data.
		std::string details{};///< Details about the activity.
		ButtonData buttons{};///< Button Data.
		ActivityType type{};///< Activity data.
		AssetsData assets{};///< Assets data.
		std::string state{};///< The player's current party status.
		std::string name{};///< Name of the activity.
		int32_t flags{ 0 };///< Flags.
		std::string url{};///< Url associated with the activity.
		EmojiData emoji{};///< Emoji associated with the activity.
		PartyData party{};///< Party data.
	};

	/// Client status data. \brief Client status data.
	struct ClientStatusData {
		std::string desktop{};///< Desktop name.
		std::string mobile{};///< Mobile name.
		std::string web{};///< Web link.
	};

	/// Premium tier levels. \brief Premium tier levels.
	enum class PremiumTier {
		None = 0,///< None.
		Tier_1 = 1,///< Tier 1.
		Tier_2 = 2,///< Tier 2.
		Tier_3 = 3///< Tier 3.
	};

	/// Default Message notification levels. \brief Default Message notification
	/// levels.
	enum class DefaultMessageNotificationLevel {
		All_Messages = 0,///< All messages.
		Only_Mentions = 1///< Only mentions.
	};

	/// Explicit content filter levels. \brief Explicit content filter levels.
	enum class ExplicitContentFilterLevel {
		Disabled = 0,///< Disabled.
		Members_Without_Roles = 1,///< Members without roles.
		All_Members = 2///< All members.
	};

	/// MFA levels. \brief MFA levels.
	enum class MFALevel {
		None = 0,///< None.
		Elevated = 1///< Elevated.
	};

	/// Verification levels. \brief/// Verification levels.
	enum class VerificationLevel {
		None = 0,///< None.
		Low = 1,///< Low.
		Medium = 2,///< Medium.
		High = 3,///< High.
		Very_High = 4///< Very high.
	};

	/// Welcome screen Channel data. \brief Welcome screen Channel data.
	struct WelcomeScreenChannelData {
		std::string description{};///< Description of the welcome Channel.
		std::string emojiName{};///< Emoji name for the Channel.
		std::string channelId{};///< Id of the welcome Channel.
		uint64_t emojiId{};///< Emoji id for the Channel.
	};

	/// Welcome screen data. \brief Welcome screen data.
	struct WelcomeScreenData {
		std::vector<WelcomeScreenChannelData> welcomeChannels{};///< Welcome screen Channel data.
		std::string description{};///< Description of the welcome screen.
	};

	/// Presence update data. \brief Presence update data.
	struct PresenceUpdateData {
		std::vector<ActivityData> activities{};///< Array of activities.
		ClientStatusData clientStatus{};///< Current client status.
		std::string status{};///< Status of the current presence.
		std::string guildId{};///< Guild id for the current presence.
		UserData user{};///< User data for the current presence.
	};

	/// Stage instance privacy levels. \brief Stage instance privacy levels.
	enum class StageInstancePrivacyLevel {
		Public = 1,///< Public.
		Guild_Only = 2///< Guild only.
	};

	/// Stage instance data. \brief Stage instance data.
	class StageInstanceData : public DiscordEntity {
	  public:
		StageInstancePrivacyLevel privacyLevel{ 0 };///< Privacy level of the Channel.
		bool discoverableDisabled{ false };///< Is it discoverable?
		std::string channelId{};///< The Channel's id.
		std::string topic{};///< The topic of the StageInstance.
		std::string guildId{};///< The Guild id for which the Channel exists in.

		virtual ~StageInstanceData() = default;
	};

	/// Sticker types. \brief Sticker types.
	enum class StickerType {
		Standard = 1,///< Standard.
		Guild = 2///< Guild.
	};

	enum class StickerFlags { Available = 1 << 0 };

	/// Data representing a single Sticker. \brief Data representing a single Sticker.
	class StickerData : public DiscordEntity {
	  public:
		void setAvailable(bool enabled) {
			if (enabled) {
				this->stickerFlags |= static_cast<int8_t>(StickerFlags::Available);
			} else {
				this->stickerFlags &= ~static_cast<int8_t>(StickerFlags::Available);
			}
		}

		bool getAvailable() {
			return this->stickerFlags & static_cast<int8_t>(StickerFlags::Available);
		}

		StickerFormatType formatType{};///< Format type.
		std::string description{};///< Description of the Sticker.
		int8_t stickerFlags{ 0 };///< Sticker flags.
		int32_t nsfwLevel{ 0 };///< NSFW warning level.
		int32_t sortValue{ 0 };///< Where in the stack of stickers it resides.
		std::string guildId{};///< The Guild id for which the Sticker exists in.
		std::string packId{};///< Pack id of the Sticker.
		std::string asset{};///< Asset value for the Sticker.
		std::string name{};///< The Sticker's name.
		std::string tags{};///< Tags for the Sticker to use.
		StickerType type{};///< The type of Sticker.
		UserData user{};///< The User that uploaded the Guild Sticker.

		virtual ~StickerData() = default;
	};

	/// Data representing a single Guild preview. \brief Data representing a single Guild preview.
	struct GuildPreviewData {
		int32_t approximatePresenceCount{ 0 };
		std::vector<std::string> features{};
		std::vector<StickerData> stickers{};
		int32_t approximateMemberCount{ 0 };
		std::vector<EmojiData> emojis{};
		std::string discoverySplash{};
		std::string description{};
		std::string splash{};
		std::string name{};
		std::string icon{};
		std::string id{};
	};

	/// Afk timeout durations. \brief Afk timeout durations.
	enum class AfkTimeOutDurations {
		Shortest = 60,///< Shortest.
		Short = 300,///< Short.
		Medium = 900,///< Medium.
		Long = 1800,///< Long.
		Longest = 3600///< Longest.
	};

	/// Guild NSFW level. \brief Guild NSFW level.
	enum class GuildNSFWLevel {
		Default = 0,///< Default.
		Explicit = 1,///< Explicit.
		Safe = 2,///< Safe.
		Age_Restricted = 3///< Age restricted.
	};

	/// System channel flags. \brief System channel flags.
	enum class SystemChannelFlags {
		Suppress_Join_Notifications = 1 << 0,///< Suppress member join notifications.
		Suppress_Premium_Subscriptions = 1 << 1,///< Suppress server boost notifications.
		Suppress_Guild_Reminder_Notifications = 1 << 2,///< Suppress server setup tips.
		Suppress_Join_Notification_Replies = 1 << 3///< Hide member join sticker reply buttons.
	};

	/// Guild flags. \brief Guild flags.
	enum class GuildFlags : int8_t {
		WidgetEnabled = 1 << 0,///< Widget enabled.
		Unavailable = 1 << 1,///< Unavailable.
		Owner = 1 << 2,///< Owner.
		Large = 1 << 3,///< Large.
		Premium_Progress_Bar_Enabled = 1 << 4///< Premium progress bar enabled
	};

	/// Data structure representing a single Guild. \brief Data structure representing a single Guild.
	class GuildData : public DiscordEntity {
	  public:
		DefaultMessageNotificationLevel defaultMessageNotifications{};///< Default Message notification level.
		std::unordered_map<std::string, PresenceUpdateData> presences{};///< Array of presences for each GuildMember.
		std::unordered_map<std::string, VoiceStateData> voiceStates{};///< Array of Guild-member voice-states.
		std::vector<std::unique_ptr<GuildMemberData>> members{};///< Array of GuildMembers.
		std::vector<std::unique_ptr<ChannelData>> channels{};///< Array of Guild channels.
		GuildNSFWLevel nsfwLevel{ GuildNSFWLevel::Default };///< NSFW warning level.
		ExplicitContentFilterLevel explicitContentFilter{};///< Explicit content filtering level, y default.
		std::vector<std::unique_ptr<RoleData>> roles{};///< Array of Guild roles.
		SystemChannelFlags systemChannelFlags{};///< System Channel flags.
		int32_t premiumSubscriptionCount{ 0 };///< Premium subscription count.
		int32_t approximatePresenceCount{ 0 };///< Approximate quantity of presences.
		VerificationLevel verificationLevel{};///< Verification level required.
		std::string publicUpdatesChannelId{};///< Id of the public updates Channel.
		std::vector<std::string> features{};///< List of Guild features.
		int32_t approximateMemberCount{ 0 };///< Approximate member count.
		WelcomeScreenData welcomeScreen{};///< Welcome screen for the Guild.
		int32_t maxVideoChannelUsers{ 0 };///< Maximum quantity of users per video Channel.
		AfkTimeOutDurations afkTimeOut{};///< Time for an individual to time out as afk.
		std::string discoverySplash{};///< Link to the discovery image's splash.
		std::string preferredLocale{};///< Preferred locale, for voice chat servers.
		std::string widgetChannelId{};///< Channel id for the Guild's widget.
		std::string systemChannelId{};///< Channel id for the Guild's system Channel.
		std::string rulesChannelId{};///< Channel id for the Guild's rules Channel.
		std::string applicationId{};///< The current application id.
		std::string vanityUrlCode{};///< Vanity Url code, if applicable.
		std::string afkChannelId{};///< Channel if of the "afk" Channel.
		std::string description{};///< Description of the Guild.
		std::string permissions{};///< Current Permissions for the bot in the Guild.
		PremiumTier premiumTier{};///< What is the premium tier?
		int32_t maxPresences{ 0 };///< Max number of presences allowed.
		TimeStamp joinedAt{ "" };///< When the bot joined this Guild.
		int32_t memberCount{ 0 };///< Member count.
		int32_t maxMembers{ 0 };///< Max quantity of members.
		std::string iconHash{};///< Url to the Guild's icon.
		std::string ownerId{};///< User id of the Guild's owner.
		std::string region{};///< Region of the world where the Guild's servers are.
		std::string splash{};///< Url to the Guild's splash.
		std::string banner{};///< Url to the Guild's banner.
		MFALevel mfaLevel{};///< MFA level.
		std::string icon{};///< Url to the Guild's icon.
		std::string name{};///< The Guild's name.
		int8_t flags{ 0 };///< Guild flags.

		GuildData() = default;

		void initialize(bool doWeShowIt);

		virtual ~GuildData() = default;
	};

	/// Guild scheduled event privacy levels. \brief Guild scheduled event privacy levels.
	enum class GuildScheduledEventPrivacyLevel {
		Public = 1,///< Public.
		Guild_Only = 2///< Guild only.
	};

	/// GuildScheduledEventStatus. \brief GuildScheduledEventStatus.
	enum class GuildScheduledEventStatus {
		Scheduled = 1,///< Scheduled.
		Active = 2,///< Active.
		Completed = 3,///< Completed.
		Canceled = 4///< Cancelled.
	};

	/// Guild scheduled event entity types. \brief Guild scheduled event entity types.
	enum class GuildScheduledEventEntityType {
		None = 0,///< None.
		State_Instance = 1,///< Stage instance.
		Voice = 2,///< Voice.
		External = 3///< External.
	};

	/// Guild scheduled event entity metadata. \brief Guild scheduled event entity metadata.
	struct GuildScheduledEventMetadata {
		std::string location{};
	};

	/// Data representing a Guild Scheduled Event. \brief Data representing a Guild Scheduled Event.
	class GuildScheduledEventData : public DiscordEntity {
	  public:
		GuildScheduledEventPrivacyLevel privacyLevel{};///< The privacy level of the scheduled event.
		GuildScheduledEventMetadata entityMetadata{};///< Additional metadata for the Guild scheduled event.
		GuildScheduledEventEntityType entityType{};///< The type of the scheduled event.
		GuildScheduledEventStatus status{};///< The status of the scheduled event.
		std::string scheduledStartTime{};///< The time the scheduled event will start.
		std::string scheduledEndTime{};///< The time the scheduled event will end, required if entity_type is External.
		std::string description{};///< The description of the scheduled event(1 - 1000 characters.
		uint32_t userCount{ 0 };///< The number of users subscribed to the scheduled event.
		std::string creatorId{};///< The id of the User that created the scheduled event *.
		std::string channelId{};///< The Channel id in which the scheduled event will be hosted, or null if scheduled entity type is External.
		std::string entityId{};///< The id of an entity associated with a Guild scheduled event.
		std::string guildId{};///< The Guild id which the scheduled event belongs to.
		std::string name{};///< The name of the scheduled event(1 - 100 characters).
		UserData creator{};///< The User that created the scheduled event.

		virtual ~GuildScheduledEventData() = default;
	};

	/// Data representing a single GuildScheduledEventUser. \brief Data representing a single GuildScheduledEventUser.
	struct GuildScheduledEventUserData {
		std::string guildScheduledEventId{};///< The scheduled event id which the User subscribed to/
		GuildMemberData member{};///< Guild member data for this User for the Guild which this event belongs to, if any.
		UserData user{};///< User which subscribed to an event.
	};

	/// Invite data. \brief Invite data.
	struct InviteData {
		GuildScheduledEventData guildScheduledEvent{};///< Scheduled Guild event.
		int32_t approximatePresenceCount{ 0 };///< Approximate presence count.
		ApplicationData targetApplication{};///< Application data.
		int32_t approximateMemberCount{ 0 };///< Approximate member count.
		StageInstanceData stageInstance{};///< Stage instance data.
		TimeStamp createdAt{ "" };///< Time it was created at.
		TimeStamp expiresAt{ "" };///< When the invite expires.
		int32_t targetType{ 0 };///< Target type.
		bool temporary{ false };///< Is it temporary?
		UserData targetUser{};///< Target User of the invite.
		ChannelData channel{};///< Channel data of the Channel that the invite is for.
		std::string guildId{};///< The Guild this invite is for.
		int32_t maxUses{ 0 };///< Max number of uses.
		int32_t maxAge{ 0 };///< Maximum age of the invite.
		std::string code{};///< Unique invite code.
		UserData inviter{};///< The User who created the invite.
		GuildData guild{};///< Guild data of the Channel that the invite is for.
		int32_t uses{ 0 };///< The current number of uses.
	};

	/// Represents a Guild Template. \brief Represents a Guild Template.
	struct GuildTemplateData {
		GuildData serializedSourceGuild{};///< The Guild snapshot this template contains.
		std::string sourceGuildId{};///< The ID of the Guild this template is based on.
		std::string description{};///< The description for the template.
		uint32_t usageCount{ 0 };///< Number of times this template has been used.
		std::string creatorId{};///< The ID of the User who created the template.
		std::string createdAt{};///< When this template was created.
		std::string updatedAt{};///< When this template was last synced to the source Guild.
		bool isDirty{ false };///< Whether the template has unsynced changes.
		std::string code{};///< The template code(unique ID).
		std::string name{};///< Template name.
		UserData creator{};///< The User who created the template.
	};

	/// Invite target types. \brief Invite target types.
	enum class InviteTargetTypes {
		Stream = 1,///< Stream.
		Embedded_Application = 2///< Embedded application.
	};

	/// WebHook types. \brief WebHook types.
	enum class WebHookType {
		Incoming = 1,///< Incoming.
		Channel_Follower = 2,///< Channel follower.
		Application = 3///< Application.
	};

	/// WebHook data. \brief WebHook data.
	class WebHookData : public DiscordEntity {
	  public:
		ChannelData sourceChannel{};///< Channel for which th WebHook was issued.
		std::string applicationId{};///< Application id.
		GuildData sourceGuild{};///< Source Guild id.
		std::string channelId{};///< Channel id for which the WebHook was issued.
		WebHookType type{ 0 };///< Type of WebHook.
		std::string guildId{};///< Guild id for which the WebHook was issued.
		std::string avatar{};///< Avatar of the WebHook.
		std::string token{};///< Token of the WebHook.
		std::string name{};///< Name of the WebHook.
		std::string url{};///< Url of the WebHook.
		UserData user{};///< User which create the WebHook.

		virtual ~WebHookData() = default;
	};

	/// Audit log data. \brief Audit log data.
	class AuditLogData {
	  public:
		auto getAuditLogData(const std::string& userIdOfChanger, AuditLogEvent auditLogType) {
			for (auto& value: this->auditLogEntries) {
				if (value.id == userIdOfChanger && value.actionType == auditLogType) {
					return value;
				}
			}
			return AuditLogEntryData();
		}
		auto getAuditLogData(AuditLogEvent auditLogType, const uint64_t& userIdOfTarget) {
			for (auto& value: this->auditLogEntries) {
				if (value.targetId == userIdOfTarget && value.actionType == auditLogType) {
					return value;
				}
			}
			return AuditLogEntryData();
		}
		std::vector<GuildScheduledEventData> guildScheduledEvents{};///< Array of guild scheduled event objects.
		std::vector<AuditLogEntryData> auditLogEntries{};///< Array of audit log entry objects.
		std::vector<IntegrationData> integrations{};///< Array of partial integration objects.
		std::vector<WebHookData> webhooks{};///< Array of webhook objects.
		std::vector<ChannelData> threads{};///< Array of thread-specific channel objects.
		std::vector<UserData> users{};///< Array of user objects.
	};

	/// For removing a reaction. \brief For removing a reaction.
	struct ReactionRemoveData {
		std::string channelId{};
		std::string messageId{};
		std::string guildId{};
		uint64_t userId{};
		EmojiData emoji{};
	};

	/// For storing Interaction-related values. \brief For storing Interaction-related values.
	struct InteractionPackageData {
		std::string interactionToken{};
		std::string applicationId{};
		std::string interactionId{};
	};

	/// For storing Message-related values. \brief For storing Message-related values.
	struct MessagePackageData {
		std::string channelId{};
		std::string messageId{};
	};

	/// Data structure representing an ApplicationCommand's option choice. \brief Data structure representing an ApplicationCommand's option choice.
	struct ApplicationCommandOptionChoiceData {
		std::unordered_map<std::string, std::string> nameLocalizations{};///< Dictionary with keys in available locales Localization dictionary for the name field.
		std::string value{};///< The value of the option.
		std::string name{};///< The name of the current choice.
	};

	/// Data structure representing an ApplicationCommand's option. \brief Data structure representing an ApplicationCommand's option.
	struct ApplicationCommandOptionData {
		std::unordered_map<std::string, std::string> descriptionLocalizations{};///< Dictionary for the description localizations field.
		std::unordered_map<std::string, std::string> nameLocalizations{};///< Dictionary for the name localizations field.
		std::vector<ApplicationCommandOptionChoiceData> choices{};///< A std::vector of possible choices for the current ApplicationCommand option.
		std::vector<ApplicationCommandOptionData> options{};///< A std::vector of possible options for the current ApplicationCommand option.
		std::vector<ChannelType> channelTypes{};///< Set when the ApplicationCommand option type is set to Channel.
		ApplicationCommandOptionType type{};///< The type of command option.
		bool autocomplete{ false };///< If autocomplete interactions are enabled for this STRING, INTEGER, or NUMBER type option.
		std::string description{};///< A description of the current ApplicationCommand option.
		bool required{ false };///< If the parameter is required or optional -- default false.
		int32_t minValue{ 0 };///< If the option is an INTEGER or NUMBER type, the minimum value permitted.
		int32_t maxValue{ 0 };///< If the option is an INTEGER or NUMBER type, the maximum value permitted.
		std::string name{};///< Name of the current ApplicationCommand option.
	};

	/// Representing "TypingStart" data. \brief Representing "TypingStart" data.
	struct TypingStartData {
		GuildMemberData member{};
		int32_t timestamp{ 0 };
		std::string channelId{};
		std::string guildId{};
		uint64_t userId{};
	};

	/// YouTube format data. \brief YouTube format data.
	struct YouTubeFormat {
		std::string signatureCipher{};
		std::string audioSampleRate{};
		int32_t averageBitrate{ 0 };
		uint64_t contentLength{ 0 };
		bool doWeGetSaved{ false };
		std::string audioQuality{};
		std::string downloadUrl{};
		std::string signature{};
		std::string mimeType{};
		std::string quality{};
		std::string codecs{};
		std::string aitags{};
		int32_t bitrate{ 0 };
		int32_t height{ 0 };
		int32_t width{ 0 };
		int32_t itag{ 0 };
		int32_t fps{ 0 };
	};

	/// Application command types. \brief Application command types.
	enum class ApplicationCommandType {
		Chat_Input = 1,///< Chat input.
		User = 2,///< User.
		Message = 3///< Message.
	};

	/// User command Interaction data. \brief User command Interaction data.
	struct UserCommandInteractionData {
		std::string targetId{};///< The target User's id.
	};

	/// Message command interacction data. \brief Message command interacction data.
	struct MessageCommandInteractionData {
		std::string targetId{};///< The target Message's id.
	};

	/// Component types. \brief Component types.
	enum class ComponentType {
		ActionRow = 1,///< A container for other components.
		Button = 2,///< A button object.
		SelectMenu = 3,///< A select menu for picking from choices.
		TextInput = 4///< A text input object
	};

	/// Component Interaction data. \brief Component Interaction data.
	struct ComponentInteractionData {
		std::vector<std::string> values{};///< The values of the components.
		ComponentType componentType{};///< The type of component.
		std::string customId{};///< The custom id of the Interaction entity.
	};

	/// Modal interaction data, for inputs from text modals. \brief Modal interaction data, for inputs from text modals.
	struct ModalInteractionData {
		std::string customIdSmall{};///< The custom id of a particular modal input.
		std::string customId{};///< The custom id of the Interaction entity.
		std::string value{};///< The input value of the modal.
	};

	/// Allowable mentions for a Message. \brief Allowable mentions for a Message.
	struct AllowedMentionsData {
		std::vector<std::string> parse{};///< A std::vector of allowed mention types to parse from the content.
		std::vector<std::string> roles{};///< Array of role_ids to mention (Max size of 100)
		std::vector<std::string> users{};///< Array of user_ids to mention (Max size of 100)
		bool repliedUser{ false };///< For replies, whether to mention the author of the Message being replied to (default false).
	};

	/// Interaction types. \brief Interaction types.
	enum class InteractionType {
		Ping = 1,///< Ping.
		Application_Command = 2,///< Application command.
		Message_Component = 3,///< Message component.
		Application_Command_Autocomplete = 4,///< Application command autocomplete.
		Modal_Submit = 5///< Modal submission.
	};

	/// Represents a single selection from a select-menu. \brief Represents a single selection from a select-menu.
	struct SelectOptionData {
		std::string description{};///< Description of the select-menu-option.
		bool _default{ false };///< Is it the default option?
		std::string label{};///< A visible label for the select-menu-option.
		std::string value{};///< A value for identifying the option.
		EmojiData emoji{};///< An optional emoji to put on it.
	};

	/// Button styles. \brief Button styles.
	enum class ButtonStyle {
		Primary = 1,///< Primary.
		Success = 3,///< Success.
		Secondary = 2,///< Secondary.
		Danger = 4,///< Danger.
		Link = 5///< Link.
	};

	/// Represents a single Message-component. \brief Represents a single Message-component.
	struct ComponentData {
		std::vector<SelectOptionData> options{};///< Aray of select options the choices in the select, max 25.
		std::string placeholder{};///< Custom placeholder text if nothing is selected, max 100 characters.
		std::string customId{};///< A developer-defined identifier for the component, max 100 characters.
		int32_t minValues{ 0 };///< The minimum number of items that must be chosen; default 1, min 0, max 25.
		int32_t maxValues{ 0 };///< The maximum number of items that can be chosen; default 1, max 25.
		bool disabled{ false };///< Whether the component is disabled, default false.
		int32_t minLength{ 0 };///< The minimum input length for a text input.
		int32_t maxLength{ 0 };///< The maximum input length for a text input.
		bool required{ false };///< Whether this component is required to be filled.
		ComponentType type{};///< Integer component type.
		std::string label{};///< The label for this component.
		std::string value{};///< A pre-filled value for this component.
		std::string title{};///< Url, for url types.
		std::string url{};///< Url, for url types.
		EmojiData emoji{};///< Emoji name, id, and animated.
		int32_t style{};///< One of button styles.
	};

	/// Action row data of Message components. \brief Action row data of Message components.
	struct ActionRowData {
		std::vector<ComponentData> components{};///< Array of components to make up the action-row.
	};

	/// Interaction callback types. \brief Interaction callback types.
	enum class InteractionCallbackType {
		Pong = 1,///< ACK a Ping.
		Channel_Message_With_Source = 4,///< Respond to an interaction with a message.
		Deferred_Channel_Message_With_Source = 5,///< ACK an interaction and edit a response later, the user sees a loading state.
		Deferred_Update_Message = 6,///< For components, ACK an interaction and edit the original message later; the user does not see a loading state.
		Update_Message = 7,///< For components, edit the message the component was attached to.
		Application_Command_Autocomplete_Result = 8,///< Respond to an autocomplete interaction with suggested choices.
		Modal = 9///< Respond to an interaction with a popup modal.
	};

	/// Interaction ApplicationCommand callback data. \brief Interaction ApplicationCommand callback data.
	struct InteractionCallbackData {
		std::vector<ApplicationCommandOptionChoiceData> choices{};///< Autocomplete choices(max of 25 choices).
		std::vector<AttachmentData> attachments{};///< Array of partial attachment objects attachment objects with filename and description.
		std::vector<ActionRowData> components{};///< Message components.
		AllowedMentionsData allowedMentions{};///< Allowed mentions data.
		std::vector<EmbedData> embeds{};///< Message embeds.
		std::vector<File> files{};///< Files for uploading.
		std::string customId{};///< A developer-defined identifier for the component, max 100 characters.
		std::string content{};///< Message content.
		std::string title{};///< The title of the popup modal.
		int32_t flags{ 0 };///< Flags.
		bool tts{ false };///< Is it TTS?
	};

	/// Data structure representing an ApplicationCommand. \brief Data structure representing an ApplicationCommand.
	class ApplicationCommandData : public DiscordEntity {
	  public:
		std::unordered_map<std::string, std::string> descriptionLocalizations{};///< Dictionary with keys in available locales Localization dictionary for name field.
		std::unordered_map<std::string, std::string> nameLocalizations{};///< Dictionary with keys in available locales Localization dictionary for name field.
		std::vector<ApplicationCommandOptionData> options{};///< A std::vector of possible options for the current ApplicationCommand.
		std::string defaultMemberPermissions{};///< Set of permissions represented as a bit set all
		ApplicationCommandType type{};///< The type of ApplicationCommand.
		std::string applicationId{};///< The current application id.
		bool dmPermission{ false };///< Indicates whether the command is available in DMs with the app, only for globally - scoped commands.
		std::string description{};///< A description of the current ApplicationCommand.
		std::string guildId{};///< (Where applicable) a Guild id for which guild to assign this ApplicationCommand to.
		std::string version{};///< An autoincremented version.
		std::string name{};///< Name of the current ApplicationCommand.

		virtual ~ApplicationCommandData() = default;
	};

	/// Function data for repeated functions to be loaded. \brief Function data for repeated functions to be loaded.
	struct RepeatedFunctionData {
		std::function<void(DiscordCoreClient*)> function{ nullptr };///< The std::function pointer to be loaded.
		uint32_t intervalInMs{ 0 };///< The time interval at which to call the std::function.
		bool repeated{ false };///< Whether or not the std::function is repeating.
		uint64_t dummyArg{ 0 };
	};

	/// Channel mention data. \brief Channel mention data.
	class ChannelMentionData : public DiscordEntity {
	  public:
		std::string guildId{};///< The id of the Guild where it took place.
		std::string name{};///< The name of the Channel that was mentioned.
		ChannelType type{};///< The type of Channel that was mentioned.

		virtual ~ChannelMentionData() = default;
	};

	/// Data for when some Channel pins are updated. \brief Data for when some Channel pins are updated.
	struct ChannelPinsUpdateEventData {
		TimeStamp lastPinTimestamp{ "" };///< The time of the last pinned Message.
		std::string channelId{};///< The id of the Channel within which the Message was pinned.
		std::string guildId{};///< The id of the Guild within which the Message was pinned.
	};

	/// Data for when threads are synced. \brief Data for when threads are synced.
	struct ThreadListSyncData {
		std::vector<ThreadMemberData> members{};///< Array of members that are a part of the Thread.
		std::vector<std::string> channelIds{};///< The parent Channel ids whose threads are being synced. If omitted, then threads were synced for entire Guild.
		std::vector<ChannelData> threads{};///< All active threads in the given channels that the current User can access.
		std::string guildId{};///< The id of the Guild for which the threads are being synced.
	};

	/// Represents a Thread-members-update. \brief Represents a Thread-members-update.
	class ThreadMembersUpdateData : public DiscordEntity {
	  public:
		std::vector<ThreadMemberData> addedMembers{};///< New members added to the Thread.
		std::vector<std::string> removedMemberIds{};///< Members who have been removed.
		int32_t memberCount{ 0 };///< Number of Guild-members in the Thread.
		std::string guildId{};///< Guild id of the Thread.

		virtual ~ThreadMembersUpdateData() = default;
	};

	/// Message Interaction data. \brief Message Interaction data.
	class MessageInteractionData : public DiscordEntity {
	  public:
		GuildMemberData member{};
		InteractionType type{};
		std::string name{};
		UserData user{};

		virtual ~MessageInteractionData() = default;
	};

	/// Message types. \brief Message types.
	enum class MessageType {
		Default = 0,///< Default.
		Recipient_Add = 1,///< Recipient add.
		Recipient_Remove = 2,///< Recipient remove.
		Call = 3,///< Call.
		Channel_Name_Change = 4,///< Channel name change.
		Channel_Icon_Change = 5,///< Channel icon change.
		Channel_Pinned_Message = 6,///< Channel pinned Message.
		Guild_Member_Join = 7,///< Guild memeber join.
		User_Premium_Guild_Subscription = 8,///< User premium Guild subscription.
		User_Premium_Guild_Subscription_Tier_1 = 9,///< User premium Guild subscription tier 1.
		User_Premium_Guild_Subscription_Tier_2 = 10,///< User premium Guild subscription tier 2.
		User_Premium_Guild_Subscription_Tier_3 = 11,///< User premium Guild subscription tier 3.
		Channel_Follow_Add = 12,///< Channel follow add.
		Guild_Discovery_Disqualified = 14,///< Guild discovery disqualified.
		Guild_Discovery_Requalified = 15,///< Guild discovery requalified.
		Guild_Discovery_Grace_Period_Initial_Warning = 16,///< Guild discovery grade period initial warning.
		Guild_Discovery_Grace_Period_Final_Warning = 17,///< Guild discovery grade period final warning.
		Thread_Created = 18,///< Thread created.
		Reply = 19,///< Reply.
		Chat_Input_Command = 20,///< Chat input command.
		Thread_Starter_Message = 21,///< Thread starter Message.
		Guild_Invite_Reminder = 22,///< Guild invite reminder.
		Context_Menu_Command = 23///< Context menu command.
	};

	/// Message flags. \brief Message flags.
	enum class MessageFlags {
		Crossposted = 1ull << 0,///< Crossposted.
		Is_Crosspost = 1ull << 1,///< Is crosspost.
		Suppress_Embeds = 1ull << 2,///< Supress embeds.
		Source_Message_Deleted = 1ull << 3,///< Source Message deleted.
		Urgent = 1ull << 4,///< Urgent.
		Has_Thread = 1ull << 5,///< Has Thread.
		Ephemeral = 1ull << 6,///< Ephemeral.
		Loading = 1ull << 7///< Loading.
	};

	/// Sticker item types. \brief Sticker item types.
	enum class StickerItemType {
		Png = 1,///< Png.
		Apng = 2,///< Apng.
		Lottie = 3///< Lottie.
	};

	/// Represents a forum thread message. \brief Represents a forum thread message.
	struct ForumThreadMessageData {
		std::vector<AttachmentData> attachments{};///< Array of partial attachment objects attachment objects with filename.
		std::vector<ActionRowData> components{};///< Array of message component objects the components to include with the message.
		AllowedMentionsData allowedMentions{};///< Allowed mention object allowed mentions for the message.
		std::vector<std::string> stickerIds{};///< Array of snowflakes IDs of up to 3 stickers in the server to send in the message.
		std::vector<EmbedData> embeds{};///< Array of embed objects	embedded rich content (up to 6000 characters).
		std::vector<File> files{};///< File contents the contents of the file being sent one of content, file, embed(s), sticker_ids.
		std::string content{};///< The message contents (up to 2000 characters).
		int32_t flags{ 0 };///< Flags to be set for the message.
	};

	/// Message Sticker item data. \brief Message Sticker item data.
	class StickerItemData : public DiscordEntity {
	  public:
		StickerItemType formatType{};///< Message Sticker item type.
		std::string name{};///< The name of the Sticker.

		virtual ~StickerItemData() = default;
	};

	/// The core of a Message's data structure. \brief The core of a Message's data structure.
	class MessageDataOld : public DiscordEntity {
	  public:
		std::vector<ChannelMentionData> mentionChannels{};///< array of Channel mention data.
		std::vector<StickerItemData> stickerItems{};///< Array of Message Sticker item data.
		std::vector<AttachmentData> attachments{};///< Array of attachment data.
		MessageReferenceData messageReference{};///< Message reference data.
		std::vector<std::string> mentionRoles{};///< std::vector of "mention roles" ids.
		std::vector<ActionRowData> components{};///< Array of action row data.
		std::vector<ReactionData> reactions{};//< Array of reaction data.
		MessageInteractionData interaction{};///< Message Interaction data.
		std::vector<StickerData> stickers{};///< Array of Message Sticker data.
		std::vector<UserData> mentions{};///< Array of User data, for individual's that were mentioned.
		std::vector<EmbedData> embeds{};///< Array of Message embeds.
		TimeStamp editedTimestamp{ "" };///< The time at which it was edited.
		MessageActivityData activity{};///< Message activity data.
		bool mentionEveryone{ false };///< Does the Message mention everyone?
		ApplicationData application{};///< Application data.
		std::string applicationId{};///< Application id.
		TimeStamp timestamp{ "" };///< The timestamp of when the Message was created.
		GuildMemberData member{};///< The author's Guild member data.
		std::string channelId{};///< The Channel it was sent in.
		std::string guildId{};///< The id of the Guild the Message was sent in.
		std::string content{};///< The Message's content.
		uint64_t webhookId{};///< WebHook id of the Message, if applicable.
		bool pinned{ false };///< Is it pinned?
		ChannelData thread{};///< The Thread that the Message was sent in, if applicable.
		std::string nonce{};///< Nonce.
		int32_t flags{ 0 };///< Flags.
		MessageType type{};///< Message type.
		UserData author{};///< The author's User data.
		bool tts{ false };///< Is it a text-to-speech Message?

		MessageDataOld() = default;

		virtual ~MessageDataOld() = default;
	};

	/// Data structure representing a single Message. \brief Data structure representing a single Message.
	class MessageData : public MessageDataOld {
	  public:
		std::unique_ptr<MessageDataOld> referencedMessage{ std::make_unique<MessageDataOld>() };///< The referenced Message, to reply to.

		MessageData& operator=(const MessageData& other) {
			*this->referencedMessage = *other.referencedMessage;
			this->messageReference = other.messageReference;
			this->mentionEveryone = other.mentionEveryone;
			this->mentionChannels = other.mentionChannels;
			this->editedTimestamp = other.editedTimestamp;
			this->applicationId = other.applicationId;
			this->stickerItems = other.stickerItems;
			this->mentionRoles = other.mentionRoles;
			this->application = other.application;
			this->interaction = other.interaction;
			this->attachments = other.attachments;
			this->components = other.components;
			this->timestamp = other.timestamp;
			this->channelId = other.channelId;
			this->webhookId = other.webhookId;
			this->reactions = other.reactions;
			this->activity = other.activity;
			this->mentions = other.mentions;
			this->stickers = other.stickers;
			this->content = other.content;
			this->guildId = other.guildId;
			this->member = other.member;
			this->thread = other.thread;
			this->pinned = other.pinned;
			this->author = other.author;
			this->embeds = other.embeds;
			this->nonce = other.nonce;
			this->flags = other.flags;
			this->type = other.type;
			this->tts = other.tts;
			this->id = other.id;
			return *this;
		}

		MessageData(const MessageData& other) {
			*this = other;
		}

		MessageData() = default;

		virtual ~MessageData() = default;
	};

	/// Resolved data. \brief Resolved data.
	struct ResolvedData {
		std::unordered_map<uint64_t, AttachmentData> attachments{};///< std::map of Snowflakes to attachment objects the ids and attachment objects.
		std::unordered_map<uint64_t, GuildMemberData> members{};///< std::map full of GuildMemeberData.
		std::unordered_map<uint64_t, MessageData> messages{};///< std::map full of messageData->
		std::unordered_map<uint64_t, ChannelData> channels{};///< std::map full of ChannelData.
		std::unordered_map<uint64_t, UserData> users{};///< std::map full of UserData.
		std::unordered_map<uint64_t, RoleData> roles{};///< std::map full of RoleData.
	};

	/// Represents a Sticker pack. \brief Represents a Sticker pack.
	struct StickerPackData {
		std::vector<StickerData> stickers{};///< Array of Sticker objects	the stickers in the pack.
		std::string coverStickerId{};///< Id of a Sticker in the pack which is shown as the pack's icon.
		std::string bannerAssetId{};///< Id of the Sticker pack's banner image.
		std::string description{};///< Description of the Sticker pack.
		std::string skuId{};///< Id of the pack's SKU.
		std::string name{};///< Name of the Sticker pack.
		std::string Id{};///< Id of the Sticker pack.
	};

	/// Connection visibility types. \brief Connection visibility types.
	enum class ConnectionVisibilityTypes {
		None = 0,///< None.
		Everyone = 1///< Everyone.
	};

	/// Represents a single User Connection. \brief Represents a single User Connection.
	struct ConnectionData {
		std::vector<IntegrationData> integrations{};///< An array of partial server integrations.
		ConnectionVisibilityTypes visibility{};///< Visibility of this connection.
		bool showActivity{ false };///< Whether activities related to this connection will be shown in presence updates.
		bool friendSync{ false };///< Whether friend sync is enabled for this connection.
		bool verified{ false };///< Whether the connection is verified.
		bool revoked{ false };///< Whether the connection is revoked.
		std::string name{};///< The userName of the connection account.
		std::string type{};///< The service of the connection(twitch, youtube).
		std::string id{};///< Id of the connection account.
	};

	/// ApplicationCommand Interaction data option. \brief ApplicationCommand Interaction data option.
	struct ApplicationCommandInteractionDataOption {
		std::vector<ApplicationCommandInteractionDataOption> options{};///< ApplicationCommand Interaction data options.
		ApplicationCommandOptionType type{};///< The type of ApplicationCommand options.
		std::string valueString{};///< The value if it's a std::string.
		bool valuebool{ false };///< the value if it's a bool.
		int32_t valueInt{ 0 };///< The value if it's an int32_t.
		bool focused{ false };///< 	True if this option is the currently focused option for autocomplete.
		std::string name{};///< The name of the current option.
	};

	/// ApplicationCommand Interaction data. \brief ApplicationCommand Interaction data.
	class ApplicationCommandInteractionData : public DiscordEntity {
	  public:
		std::vector<ApplicationCommandInteractionDataOption> options{};///< ApplicationCommand Interaction data options.
		ApplicationCommandType type{};///< The type of ApplicationCommand.
		ResolvedData resolved{};///< Resolved data.
		std::string guildId{};///< The guild that the command took place in.
		std::string name{};///< The name of the command.

		virtual ~ApplicationCommandInteractionData() = default;
	};

	/// Interaction data data. \brief Interaction data data.
	struct InteractionDataData {
		ApplicationCommandInteractionData applicationCommandData{};///< ApplicationCommand Interaction data.
		MessageCommandInteractionData messageInteractionData{};///< Message command Interaction data.
		UserCommandInteractionData userInteractionData{};///< User command Interaction data.
		ComponentInteractionData componentData{};///< Component Interaction data.
		ModalInteractionData modalData{};///< Modal Interaction data.
	};

	/// Interaction data. \brief Interaction data.
	class InteractionData : public DiscordEntity {
	  public:
		std::string applicationId{};///< The application's id.
		InteractionDataData data{};///< The Interaction's data.
		std::string guildLocale{};///< The guild's preferred locale, if invoked in a guild.
		std::string rawData{};///< The Interaction's raw data.
		GuildMemberData member{};///< The data of the Guild member who sent the Interaction, if applicable.
		std::string channelId{};///< The Channel the Interaction was sent in.
		InteractionType type{};///< The type of Interaction.
		MessageData message{};///< The Message that the Interaction came through on, if applicable.
		std::string guildId{};///< The Guild id of the Guild it was sent in.
		int32_t version{ 0 };///< The Interaction version.
		std::string locale{};///< The selected language of the invoking user.
		std::string token{};///< The Interaction token.
		UserData user{};///< The User data of the sender of the Interaction.

		InteractionData() = default;

		virtual ~InteractionData() = default;
	};

	/// Data from the SessionStart info. \brief Data from the SessionStart info.
	struct SessionStartData {
		uint32_t maxConcurrency{ 0 };///< The number of identify requests allowed per 5 seconds.
		uint32_t resetAfter{ 0 };///< The number of std::chrono::milliseconds after which the limit resets.
		uint32_t remaining{ 0 };///< The remaining number of session starts the current User is allowed.
		uint32_t total{ 0 };///< The total number of session starts the current User is allowed.
	};

	/// Data from the GetGatewatBot endpoint. \brief Data from the GetGatewatBot endpoint.
	struct GatewayBotData {
		SessionStartData sessionStartLimit{};///< Information on the current session start limit.
		uint32_t shards{ 0 };///< The recommended number of shards to use when connecting.
		std::string url{};///< The WSS Url that can be used for connecting to the gateway.
	};

	/// Text input style for modals. \brief Text input style for modals.
	enum class TextInputStyle {
		Short = 1,///< A single-line input.
		Paragraph = 2///< A multi-line input.
	};

	/// Input event response types. \brief Input event response types.
	enum class InputEventResponseType {
		Unset = 0,///< Unset.
		Deferred_Response = 1,
		Ephemeral_Deferred_Response = 2,///< Deferred ephemeral response.
		Interaction_Response = 3,///< Interaction response.
		Ephemeral_Interaction_Response = 4,///< Ephemeral Interaction response.
		Edit_Interaction_Response = 5,///< Interaction response edit.
		Follow_Up_Message = 6,///< Follow-up Message.
		Ephemeral_Follow_Up_Message = 7,///< Ephemeral follow-up Message.
		Edit_Follow_Up_Message = 8,///< Follow-up Message edit.
		Application_Command_AutoComplete_Result = 9,///< Respond to an autocomplete interaction with suggested choices.
		Modal_Interaction_Response = 10,///< Respond to an interaction with a popup modal.
	};

	/// Data representing a Guild Emoji Update event. \brief Data representing a Guild Emoji Update event.
	struct GuildEmojisUpdateEventData {
		std::vector<EmojiData> emojis{};
		std::string guildId{};
	};

	/// Data representing a Guild Sticker Update event. \brief Data representing a Guild Stickers Update event.
	struct GuildStickersUpdateEventData {
		std::vector<StickerData> stickers{};
		std::string guildId{};
	};

	struct GuildMembersChunkEventData {
		std::vector<PresenceUpdateData> presences{};
		std::vector<GuildMemberData> members{};
		std::vector<std::string> notFound{};
		int32_t chunkIndex{ 0 };
		int32_t chunkCount{ 0 };
		std::string guildId{};
		std::string nonce{};
	};

	/// Interaction response data. \brief Interaction response data.
	struct InteractionResponseData {
		InteractionResponseData() = default;
		InteractionCallbackData data{};///< Interaction ApplicationCommand callback data.
		InteractionCallbackType type{};///< Interaction callback type.
	};

	/// Sharding options for the library. \brief Sharding options for the library.
	struct ShardingOptions {
		int32_t numberOfShardsForThisProcess{ 1 };///< The number of shards to launch on the current process.
		int32_t startingShard{ 0 };///< The first shard to start on this process.
	};

	/// Logging options for the library. \brief Loggin options for the library.
	struct LoggingOptions {
		bool logWebSocketSuccessMessages{ false };///< Do we log the websocket success messages to cout?
		bool logWebSocketErrorMessages{ false };///< Do we log the websocket error messages to cout?
		bool logGeneralSuccessMessages{ false };///< Do we log general success messages to cout?
		bool logFFMPEGSuccessMessages{ false };///< Do we log FFMPEG success messages to cout?
		bool logGeneralErrorMessages{ false };///< Do we log general error messages to cout?
		bool logHttpSuccessMessages{ false };///< Do we log Http response success messages to cout?
		bool logFFMPEGErrorMessages{ false };///< Do we log FFMPEG error messages to cout?
		bool logHttpErrorMessages{ false };///< Do we log Http response error messages to cout?
	};

	/// For selecting the caching style of the library. \brief For selecting the caching style of the library.
	struct CacheOptions {
		bool cacheGuildMembers{ true };///< Do we cache GuildMembers?
		bool cacheChannels{ true };///< Do we cache Channels?
		bool cacheGuilds{ true };///< Do we cache Guilds?
		bool cacheRoles{ true };///< Do we cache Roles?
		bool cacheUsers{ true };///< Do we cache Users?
	};

	/// Guild application command permissions data. \brief Guild application command permissions data.
	class GuildApplicationCommandPermissionData : public DiscordEntity {
	  public:
		std::vector<ApplicationCommandPermissionData> permissions{};
		std::string applicationId{};
		std::string guildId{};

		virtual ~GuildApplicationCommandPermissionData() = default;
	};

	/**@}*/

	/// Song types. \brief Song types.
	enum class SongType {
		YouTube = 0,///< YouTube.
		SoundCloud = 1///< SoundCloud.
	};

	/// Represents a download Url. \brief Represents a download Url.
	struct DownloadUrl {
		int32_t contentSize{ 0 };
		std::string urlPath{};
	};

	/**
	 * \addtogroup voice_connection
	 * @{
	 */

	/// Represents a single frame of raw audio data. \brief Represents a single frame of raw audio data.
	struct RawFrameData {
		std::vector<int8_t> data{};///< The audio data.
		int32_t sampleCount{ -1 };///< The number of samples per this frame.
	};

	/// Represents a single frame of encoded audio data. \brief Represents a single frame of encoded audio data.
	struct EncodedFrameData {
		std::vector<int8_t> data{};///< The audio data.
		int32_t sampleCount{ -1 };///< The number of samples per this frame.
	};

	/// Audio frame types. \brief Audio frame types.
	enum class AudioFrameType {
		Unset = 0,///< Unset.
		Encoded = 1,///< Encoded.
		RawPCM = 2,///< Raw PCM.
		Skip = 3///< Skip.
	};

	/// Represents a single frame of audio data. \brief Represents a single frame of audio data.
	struct AudioFrameData {
		AudioFrameType type{ AudioFrameType::Unset };///< The type of audio frame.
		EncodedFrameData encodedFrameData{};///< To be filled if it's already encoded.
		uint64_t guildMemberId{};///< The Id of the GuildMember from which it was sent.
		RawFrameData rawFrameData{};///< To be filled if it's raw audio data.
	};

	/// A song from the various platforms. \brief A song from the various platforms.
	struct Song {
		friend class SoundCloudRequestBuilder;
		friend class YouTubeRequestBuilder;
		friend class SoundCloudAPI;
		friend class YouTubeAPI;
		friend class DataParser;
		friend SongAPI;

		SongType type{ SongType::SoundCloud };///< The type of song.

		std::vector<DownloadUrl> finalDownloadUrls{};
		std::string secondDownloadUrl{};
		std::string firstDownloadUrl{};
		std::string html5PlayerFile{};
		std::string addedByUserName{};///< The User name of the individual who added this Song to the playlist.
		int32_t contentLength{ 0 };
		std::string thumbnailUrl{};///< The Url of the thumbnail image of this Song.
		std::string html5Player{};
		std::string description{};///< A description of the Song.
		uint64_t addedByUserId{};///< The User id of the individual who added this Song to the playlist.
		std::string songTitle{};///< The title of the Song.
		std::string duration{};///< The duration of the Song.
		std::string viewUrl{};///< The url for listening to this Song through a browser.
		std::string songId{};

		virtual ~Song() = default;

	  protected:
		std::string trackAuthorization{};
		std::string playerResponse{};
		bool doWeGetSaved{ false };
		YouTubeFormat format{};
	};

	/// Song completion event data. \brief Song completion event data.
	struct SongCompletionEventData {
		GuildMemberData guildMember{};///< The sending GuildMember.
		bool wasItAFail{ false };///< Is this a replay? (Did a track recently fail to play?)
		Song previousSong{};///< The previously played Song.
		GuildData guild{};///< The sending Guild.
	};

	/// Playlist of songs and other variables. \brief Playlist of songs and other variables.
	struct Playlist {
		bool isLoopSongEnabled{ false };///< Is looping of Songs currently enabled?
		bool isLoopAllEnabled{ false };///< Is looping of the entire Playlist currently enabled?
		std::vector<Song> songQueue{};///< The list of Songs that are stored to be played.
		Song currentSong{};///< The current Song that is playing.
	};

	/**@}*/

};// namespace DiscordCoreLoader

/**@}*/
