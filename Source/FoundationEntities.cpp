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
/// FoundationEntities.cpp - Source file for the foundation entities.
/// May 22, 2022
/// https://discordcoreapi.com
/// \file FoundationEntities.cpp

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/SSLClients.hpp>
#include <discordcoreloader/DataParsingFunctions.hpp>

namespace DiscordCoreLoader {
	
	EnumConverter::operator std::vector<uint64_t>() const noexcept {
		return this->vector;
	}

	EnumConverter::operator uint64_t() const noexcept {
		return this->integer;
	}

	bool EnumConverter::isItAVector() const noexcept {
		return this->vectorType;
	}

	Jsonifier& Jsonifier::operator=(Jsonifier&& data) noexcept {
		switch (data.type) {
			case JsonType::Object: {
				this->setValue(JsonType::Object);
				*this->jsonValue.object = std::move(*data.jsonValue.object);
				break;
			}
			case JsonType::Array: {
				this->setValue(JsonType::Array);
				*this->jsonValue.array = std::move(*data.jsonValue.array);
				break;
			}
			case JsonType::String: {
				this->setValue(JsonType::String);
				*this->jsonValue.string = std::move(*data.jsonValue.string);
				break;
			}
			case JsonType::Float: {
				this->jsonValue.numberDouble = data.jsonValue.numberDouble;
				break;
			}
			case JsonType::Uint64: {
				this->jsonValue.numberUint = data.jsonValue.numberUint;
				break;
			}
			case JsonType::Int64: {
				this->jsonValue.numberInt = data.jsonValue.numberInt;
				break;
			}
			case JsonType::Bool: {
				this->jsonValue.boolean = data.jsonValue.boolean;
				break;
			}
		}
		this->string = std::move(data.string);
		this->type = data.type;
		return *this;
	}

	Jsonifier::Jsonifier(Jsonifier&& data) noexcept {
		*this = std::move(data);
	}

	Jsonifier& Jsonifier::operator=(const Jsonifier& data) noexcept {
		switch (data.type) {
			case JsonType::Object: {
				this->setValue(JsonType::Object);
				*this->jsonValue.object = *data.jsonValue.object;
				break;
			}
			case JsonType::Array: {
				this->setValue(JsonType::Array);
				*this->jsonValue.array = *data.jsonValue.array;
				break;
			}
			case JsonType::String: {
				this->setValue(JsonType::String);
				*this->jsonValue.string = *data.jsonValue.string;
				break;
			}
			case JsonType::Float: {
				this->jsonValue.numberDouble = data.jsonValue.numberDouble;
				break;
			}
			case JsonType::Uint64: {
				this->jsonValue.numberUint = data.jsonValue.numberUint;
				break;
			}
			case JsonType::Int64: {
				this->jsonValue.numberInt = data.jsonValue.numberInt;
				break;
			}
			case JsonType::Bool: {
				this->jsonValue.boolean = data.jsonValue.boolean;
				break;
			}
		}
		this->string = data.string;
		this->type = data.type;
		return *this;
	}

	Jsonifier::Jsonifier(const Jsonifier& data) noexcept {
		*this = data;
	}

	Jsonifier::operator std::string&&() noexcept {
		return std::move(this->string);
	}

	Jsonifier::operator std::string() noexcept {
		return this->string;
	}

	JsonType Jsonifier::getType() noexcept {
		return this->type;
	}

	void Jsonifier::refreshString(JsonifierSerializeType opCode) {
		this->string.clear();
		if (opCode == JsonifierSerializeType::Etf) {
			this->appendVersion();
			this->serializeJsonToEtfString(this);
		} else {
			this->serializeJsonToJsonString(this);
		}
	}

	Jsonifier& Jsonifier::operator=(EnumConverter&& data) noexcept {
		if (data.isItAVector()) {
			this->setValue(JsonType::Array);
			for (auto& value: data.operator std::vector<uint64_t>()) {
				this->jsonValue.array->emplace_back(std::move(value));
			}
		} else {
			this->jsonValue.numberUint = uint64_t{ data };
			this->type = JsonType::Uint64;
		}
		return *this;
	}

	Jsonifier::Jsonifier(EnumConverter&& data) noexcept {
		*this = std::move(data);
	}

	Jsonifier& Jsonifier::operator=(const EnumConverter& data) noexcept {
		if (data.isItAVector()) {
			this->setValue(JsonType::Array);
			for (auto& value: data.operator std::vector<uint64_t>()) {
				this->jsonValue.array->emplace_back(value);
			}
		} else {
			this->jsonValue.numberUint = uint64_t{ data };
			this->type = JsonType::Uint64;
		}
		return *this;
	}

	Jsonifier::Jsonifier(const EnumConverter& data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(std::string&& data) noexcept {
		this->setValue(JsonType::String);
		*this->jsonValue.string = std::move(data);
		this->type = JsonType::String;
		return *this;
	}

	Jsonifier::Jsonifier(std::string&& data) noexcept {
		*this = std::move(data);
	}

	Jsonifier& Jsonifier::operator=(const std::string& data) noexcept {
		this->setValue(JsonType::String);
		*this->jsonValue.string = data;
		this->type = JsonType::String;
		return *this;
	}

	Jsonifier::Jsonifier(const std::string& data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(const char* data) noexcept {
		this->setValue(JsonType::String);
		*this->jsonValue.string = data;
		this->type = JsonType::String;
		return *this;
	}

	Jsonifier::Jsonifier(const char* data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(double data) noexcept {
		this->jsonValue.numberDouble = data;
		this->type = JsonType::Float;
		return *this;
	}

	Jsonifier::Jsonifier(double data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(float data) noexcept {
		this->jsonValue.numberDouble = data;
		this->type = JsonType::Float;
		return *this;
	}

	Jsonifier::Jsonifier(float data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(uint64_t data) noexcept {
		this->jsonValue.numberUint = data;
		this->type = JsonType::Uint64;
		return *this;
	}

	Jsonifier::Jsonifier(uint64_t data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(uint32_t data) noexcept {
		this->jsonValue.numberUint = data;
		this->type = JsonType::Uint64;
		return *this;
	}

	Jsonifier::Jsonifier(uint32_t data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(uint16_t data) noexcept {
		this->jsonValue.numberUint = data;
		this->type = JsonType::Uint64;
		return *this;
	}

	Jsonifier::Jsonifier(uint16_t data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(uint8_t data) noexcept {
		this->jsonValue.numberUint = data;
		this->type = JsonType::Uint64;
		return *this;
	}

	Jsonifier::Jsonifier(uint8_t data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(int64_t data) noexcept {
		this->jsonValue.numberInt = data;
		this->type = JsonType::Int64;
		return *this;
	}

	Jsonifier::Jsonifier(int64_t data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(int32_t data) noexcept {
		this->jsonValue.numberInt = data;
		this->type = JsonType::Int64;
		return *this;
	}

	Jsonifier::Jsonifier(int32_t data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(int16_t data) noexcept {
		this->jsonValue.numberInt = data;
		this->type = JsonType::Int64;
		return *this;
	}

	Jsonifier::Jsonifier(int16_t data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(int8_t data) noexcept {
		this->jsonValue.numberInt = data;
		this->type = JsonType::Int64;
		return *this;
	}

	Jsonifier::Jsonifier(int8_t data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(std::nullptr_t) noexcept {
		this->type = JsonType::Null;
		return *this;
	}

	Jsonifier::Jsonifier(std::nullptr_t data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(bool data) noexcept {
		this->jsonValue.boolean = data;
		this->type = JsonType::Bool;
		return *this;
	}

	Jsonifier::Jsonifier(bool data) noexcept {
		*this = data;
	}

	Jsonifier& Jsonifier::operator=(JsonType typeNew) noexcept {
		this->type = typeNew;
		this->setValue(this->type);
		return *this;
	}

	Jsonifier::Jsonifier(JsonType type) noexcept {
		*this = type;
	}

	Jsonifier& Jsonifier::operator[](typename ObjectType::key_type key) {
		if (this->type == JsonType::Null) {
			this->setValue(JsonType::Object);
			this->type = JsonType::Object;
		}

		if (this->type == JsonType::Object) {
			auto result = this->jsonValue.object->emplace(std::move(key), Jsonifier{});
			return result.first->second;
		}
		throw std::runtime_error{ "Sorry, but that item-key could not be produced/accessed." };
	}

	Jsonifier& Jsonifier::operator[](uint64_t index) {
		if (this->type == JsonType::Null) {
			this->setValue(JsonType::Array);
			this->type = JsonType::Array;
		}

		if (this->type == JsonType::Array) {
			if (index >= this->jsonValue.array->size()) {
				this->jsonValue.array->resize(index + 1);
			}

			return this->jsonValue.array->operator[](index);
		}
		throw std::runtime_error{ "Sorry, but that index could not be produced/accessed." };
	}

	template<> Jsonifier::ObjectType& Jsonifier::get() {
		return *this->jsonValue.object;
	}

	template<> Jsonifier::ArrayType& Jsonifier::get() {
		return *this->jsonValue.array;
	}

	template<> Jsonifier::StringType& Jsonifier::get() {
		return *this->jsonValue.string;
	}

	template<> Jsonifier::FloatType& Jsonifier::get() {
		return this->jsonValue.numberDouble;
	}

	template<> Jsonifier::UintType& Jsonifier::get() {
		return this->jsonValue.numberUint;
	}

	template<> Jsonifier::IntType& Jsonifier::get() {
		return this->jsonValue.numberInt;
	}

	template<> Jsonifier::BoolType& Jsonifier::get() {
		return this->jsonValue.boolean;
	}

	void Jsonifier::emplaceBack(Jsonifier&& other) noexcept {
		if (this->type == JsonType::Null) {
			this->setValue(JsonType::Array);
			this->type = JsonType::Array;
		}

		if (this->type == JsonType::Array) {
			this->jsonValue.array->emplace_back(std::move(other));
		}
	}

	void Jsonifier::emplaceBack(Jsonifier& other) noexcept {
		if (this->type == JsonType::Null) {
			this->setValue(JsonType::Array);
			this->type = JsonType::Array;
		}

		if (this->type == JsonType::Array) {
			this->jsonValue.array->emplace_back(other);
		}
	}

	void Jsonifier::serializeJsonToEtfString(const Jsonifier* dataToParse) {
		switch (dataToParse->type) {
			case JsonType::Object: {
				return this->writeEtfObject(*dataToParse->jsonValue.object);
			}
			case JsonType::Array: {
				return this->writeEtfArray(*dataToParse->jsonValue.array);
			}
			case JsonType::String: {
				return this->writeEtfString(*dataToParse->jsonValue.string);
			}
			case JsonType::Float: {
				return this->writeEtfFloat(dataToParse->jsonValue.numberDouble);
			}
			case JsonType::Uint64: {
				return this->writeEtfUint(dataToParse->jsonValue.numberUint);
			}
			case JsonType::Int64: {
				return this->writeEtfInt(dataToParse->jsonValue.numberInt);
			}
			case JsonType::Bool: {
				return this->writeEtfBool(dataToParse->jsonValue.boolean);
			}
			case JsonType::Null: {
				return this->writeEtfNull();
			}
		}
	}

	void Jsonifier::serializeJsonToJsonString(const Jsonifier* dataToParse) {
		switch (dataToParse->type) {
			case JsonType::Object: {
				return this->writeJsonObject(*dataToParse->jsonValue.object);
			}
			case JsonType::Array: {
				return this->writeJsonArray(*dataToParse->jsonValue.array);
			}
			case JsonType::String: {
				return this->writeJsonString(*dataToParse->jsonValue.string);
			}
			case JsonType::Float: {
				return this->writeJsonFloat(dataToParse->jsonValue.numberDouble);
			}
			case JsonType::Uint64: {
				return this->writeJsonInt(dataToParse->jsonValue.numberUint);
			}
			case JsonType::Int64: {
				return this->writeJsonInt(dataToParse->jsonValue.numberInt);
			}
			case JsonType::Bool: {
				return this->writeJsonBool(dataToParse->jsonValue.boolean);
			}
			case JsonType::Null: {
				return this->writeJsonNull();
			}
		}
	}

	void Jsonifier::writeJsonObject(const ObjectType& objectNew) {
		if (objectNew.empty()) {
			this->writeString("{}", 2);
			return;
		}
		this->writeCharacter('{');

		int32_t index{};
		for (auto& [key, value]: objectNew) {
			this->writeJsonString(key);
			this->writeCharacter(':');
			this->serializeJsonToJsonString(&value);

			if (index != objectNew.size() - 1) {
				this->writeCharacter(',');
			}
			index++;
		}

		this->writeCharacter('}');
	}

	void Jsonifier::writeJsonArray(const ArrayType& arrayNew) {
		if (arrayNew.empty()) {
			this->writeString("[]", 2);
			return;
		}

		this->writeCharacter('[');

		int32_t index{};
		for (auto& value: arrayNew) {
			this->serializeJsonToJsonString(&value);
			if (index != arrayNew.size() - 1) {
				this->writeCharacter(',');
			}
			index++;
		}

		this->writeCharacter(']');
	}

	void Jsonifier::writeJsonString(const StringType& stringNew) {
		this->writeCharacter('\"');
		for (auto& value: stringNew) {
			switch (static_cast<std::uint8_t>(value)) {
				case 0x08: {
					this->writeCharacter('b');
					break;
				}
				case 0x09: {
					this->writeCharacter('t');
					break;
				}
				case 0x0A: {
					this->writeCharacter('n');
					break;
				}
				case 0x0C: {
					this->writeCharacter('f');
					break;
				}
				case 0x0D: {
					this->writeCharacter('r');
					break;
				}
				case 0x22: {
					this->writeCharacter('\"');
					break;
				}
				case 0x5C: {
					this->writeCharacter('\\');
					break;
				}
				default: {
					this->writeCharacter(value);
					break;
				}
			}
		}
		this->writeCharacter('\"');
	}

	void Jsonifier::writeJsonFloat(const FloatType x) {
		auto floatValue = std::to_string(x);
		this->writeString(floatValue.data(), floatValue.size());
	}

	void Jsonifier::writeJsonBool(const BoolType jsonValueNew) {
		if (jsonValueNew) {
			this->writeString("true", 4);
		} else {
			this->writeString("false", 5);
		}
	}

	void Jsonifier::writeJsonNull() {
		this->writeString("null", 4);
	}

	void Jsonifier::writeEtfObject(const ObjectType& jsonData) {
		this->appendMapHeader(static_cast<uint32_t>(jsonData.size()));
		for (auto& [key, value]: jsonData) {
			this->appendBinaryExt(key, static_cast<uint32_t>(key.size()));
			this->serializeJsonToEtfString(&value);
		}
	}

	void Jsonifier::writeEtfArray(const ArrayType& jsonData) {
		this->appendListHeader(static_cast<uint32_t>(jsonData.size()));
		for (auto& value: jsonData) {
			this->serializeJsonToEtfString(&value);
		}
		this->appendNilExt();
	}

	void Jsonifier::writeEtfString(const StringType& jsonData) {
		this->appendBinaryExt(jsonData, static_cast<uint32_t>(jsonData.size()));
	}

	void Jsonifier::writeEtfUint(const UintType jsonData) {
		if (jsonData <= 255) {
			this->appendSmallIntegerExt(static_cast<uint8_t>(jsonData));
		} else if (jsonData <= std::numeric_limits<uint32_t>::max()) {
			this->appendIntegerExt(static_cast<uint32_t>(jsonData));
		} else {
			this->appendUnsignedLongLong(jsonData);
		}
	}

	void Jsonifier::writeEtfInt(const IntType jsonData) {
		if (jsonData <= 127 && jsonData >= -127) {
			this->appendSmallIntegerExt(static_cast<uint8_t>(jsonData));
		} else if (jsonData <= std::numeric_limits<int32_t>::max() && jsonData >= std::numeric_limits<int32_t>::min()) {
			this->appendIntegerExt(static_cast<uint32_t>(jsonData));
		} else {
			this->appendUnsignedLongLong(static_cast<uint64_t>(jsonData));
		}
	}

	void Jsonifier::writeEtfFloat(const FloatType jsonData) {
		this->appendNewFloatExt(jsonData);
	}

	void Jsonifier::writeEtfBool(const BoolType jsonData) {
		this->appendBool(jsonData);
	}

	void Jsonifier::writeEtfNull() {
		this->appendNil();
	}

	void Jsonifier::writeString(const char* data, std::size_t length) {
		this->string.append(data, length);
	}

	void Jsonifier::writeCharacter(const char charValue) {
		this->string.push_back(charValue);
	}

	bool operator==(const Jsonifier& lhs, const Jsonifier& rhs) {
		if (lhs.type != rhs.type) {
			return false;
		}
		switch (rhs.type) {
			case JsonType::Object: {
				if (*lhs.jsonValue.object != *rhs.jsonValue.object) {
					return false;
				}
				break;
			}
			case JsonType::Array: {
				if (*lhs.jsonValue.array != *rhs.jsonValue.array) {
					return false;
				}
				break;
			}
			case JsonType::String: {
				if (*lhs.jsonValue.string != *rhs.jsonValue.string) {
					return false;
				}
				break;
			}
			case JsonType::Float: {
				if (lhs.jsonValue.numberDouble != rhs.jsonValue.numberDouble) {
					return false;
				}
				break;
			}
			case JsonType::Uint64: {
				if (lhs.jsonValue.numberUint != rhs.jsonValue.numberUint) {
					return false;
				}
				break;
			}
			case JsonType::Int64: {
				if (lhs.jsonValue.numberInt != rhs.jsonValue.numberInt) {
					return false;
				}
				break;
			}
			case JsonType::Bool: {
				if (lhs.jsonValue.boolean != rhs.jsonValue.boolean) {
					return false;
				}
				break;
			}
		}
		return true;
	}

	void Jsonifier::appendBinaryExt(const std::string& bytes, uint32_t sizeNew) {
		std::string bufferNew{ static_cast<uint8_t>(EtfType::Binary_Ext) };
		storeBits(bufferNew, sizeNew);
		this->writeString(bufferNew.data(), bufferNew.size());
		this->writeString(bytes.data(), bytes.size());
	}

	void Jsonifier::appendUnsignedLongLong(const uint64_t value) {
		std::string bufferNew{};
		uint64_t jsonValueNew = value;
		bufferNew.resize(static_cast<uint64_t>(1) + 2 + sizeof(uint64_t));
		bufferNew[0] = static_cast<uint8_t>(EtfType::Small_Big_Ext);
		StopWatch stopWatch{ std::chrono::milliseconds{ 1500 } };
		uint8_t bytesToEncode = 0;
		while (jsonValueNew > 0) {
			if (stopWatch.hasTimePassed()) {
				break;
			}
			bufferNew[static_cast<uint64_t>(3) + bytesToEncode] = jsonValueNew & 0xF;
			jsonValueNew >>= 8;
			bytesToEncode++;
		}
		bufferNew[1] = bytesToEncode;
		bufferNew[2] = 0;
		this->writeString(bufferNew.data(), bufferNew.size());
	}

	void Jsonifier::appendNewFloatExt(const double FloatValue) {
		std::string bufferNew{ static_cast<unsigned char>(EtfType::New_Float_Ext) };
		const void* punner{ &FloatValue };
		storeBits(bufferNew, *static_cast<const uint64_t*>(punner));
		this->writeString(bufferNew.data(), bufferNew.size());
	}

	void Jsonifier::appendSmallIntegerExt(const uint8_t value) {
		std::string bufferNew{ static_cast<uint8_t>(EtfType::Small_Integer_Ext), static_cast<char>(value) };
		this->writeString(bufferNew.data(), bufferNew.size());
	}

	void Jsonifier::appendIntegerExt(const uint32_t value) {
		std::string bufferNew{ static_cast<uint8_t>(EtfType::Integer_Ext) };
		storeBits(bufferNew, value);
		this->writeString(bufferNew.data(), bufferNew.size());
	}

	void Jsonifier::appendListHeader(const uint32_t sizeNew) {
		std::string bufferNew{ static_cast<uint8_t>(EtfType::List_Ext) };
		storeBits(bufferNew, sizeNew);
		this->writeString(bufferNew.data(), bufferNew.size());
	}

	void Jsonifier::appendMapHeader(const uint32_t sizeNew) {
		std::string bufferNew{ static_cast<uint8_t>(EtfType::Map_Ext) };
		storeBits(bufferNew, sizeNew);
		this->writeString(bufferNew.data(), bufferNew.size());
	}

	void Jsonifier::appendBool(bool data) {
		if (data) {
			std::string bufferNew{ static_cast<uint8_t>(EtfType::Atom_Ext) };
			storeBits(bufferNew, static_cast<uint16_t>(4));
			this->writeString(bufferNew.data(), bufferNew.size());
			this->writeString("true", 4);

		} else {
			std::string bufferNew{ static_cast<uint8_t>(EtfType::Atom_Ext) };
			storeBits(bufferNew, static_cast<uint16_t>(5));
			this->writeString(bufferNew.data(), bufferNew.size());
			this->writeString("false", 5);
		}
	}

	void Jsonifier::appendVersion() {
		std::string bufferNew{ static_cast<int8_t>(formatVersion) };
		this->writeString(bufferNew.data(), bufferNew.size());
	}

	void Jsonifier::appendNilExt() {
		this->writeCharacter(static_cast<uint8_t>(EtfType::Nil_Ext));
	}

	void Jsonifier::appendNil() {
		std::string bufferNew{ static_cast<uint8_t>(EtfType::Atom_Ext) };
		storeBits(bufferNew, static_cast<uint16_t>(3));
		this->writeString(bufferNew.data(), bufferNew.size());
		this->writeString("nil", 3);
	}

	void Jsonifier::setValue(JsonType typeNew) {
		this->destroy();
		this->type = typeNew;
		switch (this->type) {
			case JsonType::Object: {
				AllocatorType<ObjectType> allocator{};
				this->jsonValue.object = AllocatorTraits<ObjectType>::allocate(allocator, 1);
				AllocatorTraits<ObjectType>::construct(allocator, this->jsonValue.object);
				break;
			}
			case JsonType::Array: {
				AllocatorType<ArrayType> allocator{};
				this->jsonValue.array = AllocatorTraits<ArrayType>::allocate(allocator, 1);
				AllocatorTraits<ArrayType>::construct(allocator, this->jsonValue.array);
				break;
			}
			case JsonType::String: {
				AllocatorType<StringType> allocator{};
				this->jsonValue.string = AllocatorTraits<StringType>::allocate(allocator, 1);
				AllocatorTraits<StringType>::construct(allocator, this->jsonValue.string);
				break;
			}
		}
	}

	void Jsonifier::destroy() noexcept {
		switch (this->type) {
			case JsonType::Object: {
				AllocatorType<ObjectType> allocator{};
				AllocatorTraits<ObjectType>::destroy(allocator, this->jsonValue.object);
				AllocatorTraits<ObjectType>::deallocate(allocator, this->jsonValue.object, 1);
				break;
			}
			case JsonType::Array: {
				AllocatorType<ArrayType> allocator{};
				AllocatorTraits<ArrayType>::destroy(allocator, this->jsonValue.array);
				AllocatorTraits<ArrayType>::deallocate(allocator, this->jsonValue.array, 1);
				break;
			}
			case JsonType::String: {
				AllocatorType<StringType> allocator{};
				AllocatorTraits<StringType>::destroy(allocator, this->jsonValue.string);
				AllocatorTraits<StringType>::deallocate(allocator, this->jsonValue.string, 1);
				break;
			}
		}
	}

	Jsonifier::~Jsonifier() noexcept {
		this->destroy();
	}
	
	void reportException(const std::string& stackTrace, UnboundedMessageBlock<std::exception>* sendBuffer, bool rethrow) {
		try {
			auto currentException = std::current_exception();
			if (currentException) {
				std::rethrow_exception(currentException);
			}
		} catch (std::exception& e) {
			if (rethrow) {
				std::rethrow_exception(std::current_exception());
				return;
			}
			if (sendBuffer) {
				sendBuffer->send(e);
			} else {
				if (stackTrace.back() == '\n') {
					std::cout << shiftToBrightRed() << stackTrace + "Error: " << e.what() << reset() << std::endl << std::endl;
				} else {
					std:: cout << shiftToBrightRed() << stackTrace + " Error: " << e.what() << reset() << std::endl << std::endl;
				}
			}
		}
	}

	std::string getISO8601TimeStamp(const std::string& year, const std::string& month, const std::string& day, const std::string& hour, const std::string& minute, const std::string& second) {
		std::string theTimeStamp{};
		theTimeStamp += year + "-";
		if (month.size() < 2) {
			theTimeStamp += "0" + month + "-";
		} else {
			theTimeStamp += month + "-";
		}
		if (day.size() < 2) {
			theTimeStamp += "0" + day;
		} else {
			theTimeStamp += day;
		}
		if (hour.size() < 2) {
			theTimeStamp += "T0" + hour;
		} else {
			theTimeStamp += "T" + hour;
		}
		if (minute.size() < 2) {
			theTimeStamp += ":0" + minute;
		} else {
			theTimeStamp += ":" + minute;
		}
		if (second.size() < 2) {
			theTimeStamp += ":0" + second;
		} else {
			theTimeStamp += ":" + second;
		}
		return theTimeStamp;
	}

	std::string convertTimeInMsToDateTimeString(uint64_t timeInMs, TimeFormat timeFormat) {
		uint64_t timeValue = timeInMs / 1000;
		time_t rawTime(timeValue);
		tm timeInfo = *localtime(&rawTime);
		std::string timeStamp{};
		timeStamp.resize(48);
		switch (timeFormat) {
			case TimeFormat::LongDate: {
				size_t sizeResponse = strftime(timeStamp.data(), 48, "%d %B %G", &timeInfo);
				timeStamp.resize(sizeResponse);
				break;
			}
			case TimeFormat::LongDateTime: {
				size_t sizeResponse = strftime(timeStamp.data(), 48, "%a %b %d %Y %X", &timeInfo);
				timeStamp.resize(sizeResponse);
				break;
			}
			case TimeFormat::LongTime: {
				size_t sizeResponse = strftime(timeStamp.data(), 48, "%T", &timeInfo);
				timeStamp.resize(sizeResponse);
				break;
			}
			case TimeFormat::ShortDate: {
				size_t sizeResponse = strftime(timeStamp.data(), 48, "%d/%m/%g", &timeInfo);
				timeStamp.resize(sizeResponse);
				break;
			}
			case TimeFormat::ShortDateTime: {
				size_t sizeResponse = strftime(timeStamp.data(), 48, "%d %B %G %R", &timeInfo);
				timeStamp.resize(sizeResponse);
				break;
			}
			case TimeFormat::ShortTime: {
				size_t sizeResponse = strftime(timeStamp.data(), 48, "%R", &timeInfo);
				timeStamp.resize(sizeResponse);
				break;
			}
			default: {
				break;
			}
		}
		return timeStamp;
	}

	std::string convertMsToDurationString(int32_t durationInMs) {
		std::string newString{};
		int32_t msPerSecond{ 1000 };
		int32_t secondsPerMinute{ 60 };
		int32_t minutesPerHour{ 60 };
		int32_t msPerMinute{ msPerSecond * secondsPerMinute };
		int32_t msPerHour{ msPerMinute * minutesPerHour };
		int32_t hoursLeft = static_cast<int32_t>(trunc(durationInMs / msPerHour));
		int32_t minutesLeft = static_cast<int32_t>(trunc((durationInMs % msPerHour) / msPerMinute));
		int32_t secondsLeft = static_cast<int32_t>(trunc(((durationInMs % msPerHour) % msPerMinute) / msPerSecond));
		if (hoursLeft >= 1) {
			newString += std::to_string(hoursLeft) + " Hours, ";
			newString += std::to_string(minutesLeft) + " Minutes, ";
			newString += std::to_string(secondsLeft) + " Seconds.";
		} else if (minutesLeft >= 1) {
			newString += std::to_string(minutesLeft) + " Minutes, ";
			newString += std::to_string(secondsLeft) + " Seconds.";
		} else {
			newString += std::to_string(secondsLeft) + " Seconds.";
		}
		return newString;
	}

	std::string convertToLowerCase(const std::string& stringToConvert) {
		std::string newString;
		for (auto& value: stringToConvert) {
			if (isupper(static_cast<int8_t>(value))) {
				newString += static_cast<int8_t>(tolower(static_cast<int8_t>(value)));
			} else {
				newString += value;
			}
		}
		return newString;
	}

	uint64_t convertTimestampToMsInteger(const std::string& timeStamp) {
		Time timeValue = Time(stoi(timeStamp.substr(0, 4)), stoi(timeStamp.substr(5, 6)), stoi(timeStamp.substr(8, 9)), stoi(timeStamp.substr(11, 12)),
			stoi(timeStamp.substr(14, 15)), stoi(timeStamp.substr(17, 18)));
		return timeValue.getTime() * 1000;
	}

	std::string base64Encode(const std::string& theString, bool url) {
		const char* base64_chars[2] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
										"abcdefghijklmnopqrstuvwxyz"
										"0123456789"
										"+/",

			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789"
			"-_" };

		size_t len_encoded = (theString.size() + 2) / 3 * 4;

		int8_t trailing_char = url ? '.' : '=';

		const char* base64_chars_ = base64_chars[url];

		std::string returnString{};
		returnString.reserve(len_encoded);

		uint64_t pos = 0;

		while (pos < theString.size()) {
			returnString.push_back(base64_chars_[(theString[static_cast<uint64_t>(pos + 0)] & 0xfc) >> 2]);

			if (static_cast<uint64_t>(pos + 1) < theString.size()) {
				returnString.push_back(base64_chars_[((theString[static_cast<uint64_t>(pos + 0)] & 0x03) << 4) + ((theString[static_cast<uint64_t>(pos + 1)] & 0xf0) >> 4)]);

				if (static_cast<uint64_t>(pos + 2) < theString.size()) {
					returnString.push_back(base64_chars_[((theString[static_cast<uint64_t>(pos + 1)] & 0x0f) << 2) + ((theString[static_cast<uint64_t>(pos + 2)] & 0xc0) >> 6)]);
					returnString.push_back(base64_chars_[theString[static_cast<uint64_t>(pos + 2)] & 0x3f]);
				} else {
					returnString.push_back(base64_chars_[(theString[static_cast<uint64_t>(pos + 1)] & 0x0f) << 2]);
					returnString.push_back(trailing_char);
				}
			} else {
				returnString.push_back(base64_chars_[(theString[static_cast<uint64_t>(pos + 0)] & 0x03) << 4]);
				returnString.push_back(trailing_char);
				returnString.push_back(trailing_char);
			}

			pos += 3;
		}

		return returnString;
	}

	std::string loadFileContents(const std::string& filePath) {
		std::ifstream file(filePath, std::ios::in | std::ios::binary);
		std::ostringstream stream{};
		stream << file.rdbuf();
		std::string theString = stream.str();
		return theString;
	}

	std::string utf8MakeValid(const std::string& inputString) {
		std::string returnString{};
		for (auto& value: inputString) {
			if (value >= 128) {
				returnString.push_back(value - 128);
			} else {
				returnString.push_back(value);
			}
		}
		return returnString;
	}

	std::string urlEncode(const std::string& inputString) {
		std::ostringstream escaped{};
		escaped.fill('0');
		escaped << std::hex;

		for (std::string::const_iterator x = inputString.begin(), n = inputString.end(); x != n; ++x) {
			std::string::value_type c = (*x);

			if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
				escaped << c;
				continue;
			}

			escaped << std::uppercase;
			escaped << '%' << std::setw(2) << int32_t(static_cast<int8_t>(c));
			escaped << std::nouppercase;
		}
		return escaped.str();
	}

	void spinLock(uint64_t timeInNsToSpinLockFor) {
		uint64_t startTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		uint64_t timePassed{ 0 };
		while (timePassed < timeInNsToSpinLockFor) {
			timePassed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - startTime;
		}
	}

	std::string getCurrentISO8601TimeStamp() {
		std::time_t result = std::time(nullptr);
		auto resultTwo = std::localtime(&result);
		std::string resultString = getISO8601TimeStamp(std::to_string(resultTwo->tm_year + 1900), std::to_string(resultTwo->tm_mon), std::to_string(resultTwo->tm_mday),
			std::to_string(resultTwo->tm_hour), std::to_string(resultTwo->tm_min), std::to_string(resultTwo->tm_sec));
		return resultString;
	}

	std::string generateBase64EncodedKey() {
		std::string returnString{};
		returnString.resize(16);
		std::mt19937_64 randomEngine{ static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) };
		for (uint32_t x = 0; x < 16; x++) {
			returnString[x] = static_cast<int8_t>((static_cast<float>(randomEngine()) / static_cast<float>(randomEngine.max())) * 255.0f);
		}
		returnString = base64Encode(returnString, false);
		return returnString;
	}

	std::string shiftToBrightGreen() {
		return std::string("\033[1;40;92m");
	}

	std::string shiftToBrightBlue() {
		return std::string("\033[1;40;96m");
	}

	std::string shiftToBrightRed() {
		return std::string("\033[1;40;91m");
	}

	bool nanoSleep(int64_t ns) {
#ifdef _WIN32
		HANDLE timer = CreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
		LARGE_INTEGER largeInt{ .QuadPart = -ns / 100 };
		if (!timer) {
			return false;
		}

		if (!SetWaitableTimerEx(timer, &largeInt, 0, NULL, NULL, NULL, 0)) {
			CloseHandle(timer);
			return false;
		}
		WaitForSingleObjectEx(timer, INFINITE, false);
		CloseHandle(timer);
#else
		std::this_thread::sleep_for(std::chrono::nanoseconds{ ns });
#endif
		return true;
	}

	std::string reset() {
		return std::string("\033[0m");
	}

	std::ostream& operator<<(std::ostream& outputSttream, const std::string& (*theFunction)( void )) {
		outputSttream << theFunction();
		return outputSttream;
	}

	bool hasTimeElapsed(const std::string& timeStamp, uint64_t days, uint64_t hours, uint64_t minutes) {
		uint64_t startTimeRaw = convertTimestampToMsInteger(timeStamp);
		auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		uint64_t secondsPerMinute = 60;
		uint64_t secondsPerHour = secondsPerMinute * 60;
		uint64_t secondsPerDay = secondsPerHour * 24;
		auto targetElapsedTime = ((days * secondsPerDay) + (hours * secondsPerHour) + (minutes * secondsPerMinute)) * 1000;
		auto actualElapsedTime = currentTime - startTimeRaw;
		if (actualElapsedTime >= targetElapsedTime) {
			return true;
		} else {
			return false;
		}
	}

	std::string getFutureISO8601TimeStamp(int32_t minutesToAdd, int32_t hoursToAdd, int32_t daysToAdd, int32_t monthsToAdd, int32_t yearsToAdd) {
		std::time_t result = std::time(nullptr);
		int32_t secondsPerMinute{ 60 };
		int32_t minutesPerHour{ 60 };
		int32_t secondsPerHour{ minutesPerHour * secondsPerMinute };
		int32_t hoursPerDay{ 24 };
		int32_t secondsPerDay{ secondsPerHour * hoursPerDay };
		int32_t daysPerMonth{ 30 };
		int32_t secondsPerMonth{ secondsPerDay * daysPerMonth };
		int32_t daysPerYear{ 365 };
		int32_t secondsPerYear{ secondsPerDay * daysPerYear };
		int32_t secondsToAdd =
			(yearsToAdd * secondsPerYear) + (monthsToAdd * secondsPerMonth) + (daysToAdd * secondsPerDay) + (hoursToAdd * secondsPerHour) + (minutesToAdd * secondsPerMinute);
		result += secondsToAdd;
		auto resultTwo = std::localtime(&result);
		std::string resultString{};
		if (resultTwo->tm_isdst) {
			if (resultTwo->tm_hour + 4 >= 24) {
				resultTwo->tm_hour = resultTwo->tm_hour - 24;
				resultTwo->tm_mday++;
			}
			resultString = getISO8601TimeStamp(std::to_string(resultTwo->tm_year + 1900), std::to_string(resultTwo->tm_mon + 1), std::to_string(resultTwo->tm_mday),
				std::to_string(resultTwo->tm_hour + 4), std::to_string(resultTwo->tm_min), std::to_string(resultTwo->tm_sec));
		} else {
			if (resultTwo->tm_hour + 5 >= 24) {
				resultTwo->tm_hour = resultTwo->tm_hour - 24;
				resultTwo->tm_mday++;
			}
			resultString = getISO8601TimeStamp(std::to_string(resultTwo->tm_year + 1900), std::to_string(resultTwo->tm_mon + 1), std::to_string(resultTwo->tm_mday),
				std::to_string(resultTwo->tm_hour + 5), std::to_string(resultTwo->tm_min), std::to_string(resultTwo->tm_sec));
		}
		return resultString;
	}

	std::string getTimeAndDate() {
		const time_t now = std::time(nullptr);
		tm time = *std::localtime(&now);
		std::string timeStamp{};
		timeStamp.resize(48);
		if (time.tm_isdst) {
			if (time.tm_hour + 4 >= 24) {
				time.tm_hour = 0 + time.tm_hour + 4 - 24;

			} else {
				time.tm_hour = time.tm_hour + 4;
			}
		} else {
			if (time.tm_hour + 5 >= 24) {
				time.tm_hour = 0 + time.tm_hour + 5 - 24;
			} else {
				time.tm_hour = time.tm_hour + 5;
			}
		}
		size_t size = strftime(timeStamp.data(), 48, "%F %R", &time);
		timeStamp.resize(size);
		return timeStamp;
	}

	std::string DiscordEntity::getCreatedAtTimestamp(TimeFormat timeFormat) {
		std::string returnString{};
		uint64_t timeInMs = (stoull(this->id) >> 22) + 1420070400000;
		returnString = convertTimeInMsToDateTimeString(timeInMs, timeFormat);
		return returnString;
	}

	WebSocketMessageReal::WebSocketMessageReal(simdjson::ondemand::value jsonObjectData) {
		this->op = getUint32(jsonObjectData, "op");

		this->s = getUint32(jsonObjectData, "s");

		this->t = getString(jsonObjectData, "t");

		if (jsonObjectData["d"].get(this->d) != simdjson::error_code::SUCCESS) {
			throw std::runtime_error{ "Faile to collect the 'd'." };
		}
	}
};
