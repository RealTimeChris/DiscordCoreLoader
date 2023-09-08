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
/// https://discordcoreloader.com
/// \file FoundationEntities.cpp

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/SSLClients.hpp>
#include <discordcoreloader/ErlParser.hpp>

namespace DiscordCoreLoader {

	DCAException::DCAException(const std::string& error, std::source_location location) noexcept : std::runtime_error(error) {
		std::stringstream stream{};
		stream << shiftToBrightRed() << "Thrown From: " << location.file_name() << " (" << std::to_string(location.line()) << ":" << std::to_string(location.column()) << ")\n";
		stream << error << DiscordCoreLoader::reset().operator std::basic_string_view<char, std::char_traits<char>>().data() << std::endl;
		*static_cast<std::runtime_error*>(this) = std::runtime_error{ stream.str() };
	}

	GuildData::operator EtfSerializer() noexcept {
		EtfSerializer data{};
		for (auto& valueNew: this->channels) {
			EtfSerializer channel{};
			channel["id"] = std::string{ valueNew.id };
			data["channels"].emplaceBack(std::move(channel));
		}

		for (auto& valueNew: this->roles) {
			EtfSerializer role{};
			role["id"] = std::string{ valueNew.id };
			data["roles"].emplaceBack(std::move(role));
		}

		for (auto& valueNew: this->members) {
			EtfSerializer member{};
			member["user"]["id"] = std::string{ valueNew.user.id };
			data["members"].emplaceBack(std::move(member));
		}
		return data;
	}

	void reportException(const std::string& currentFunctionName, std::source_location theLocation) {
		try {
			auto currentException = std::current_exception();
			if (currentException) {
				std::rethrow_exception(currentException);
			}
		} catch (const std::exception& e) {
			std::stringstream theStream{};
			theStream << shiftToBrightRed() << "Error Report: \n"
					  << "Caught At: " << currentFunctionName << ", in File: " << theLocation.file_name() << " (" << std::to_string(theLocation.line()) << ":"
					  << std::to_string(theLocation.column()) << ")"
					  << "\nThe Error: \n"
					  << e.what() << reset() << std::endl
					  << std::endl;
			auto returnString = theStream.str();
			std::cout << returnString;
		}
	}

	std::string getISO8601TimeStamp(const std::string& year, const std::string& month, const std::string& day, const std::string& hour, const std::string& minute,
		const std::string& second) {
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
		return std::string{ theTimeStamp };
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
		int32_t hoursLeft	= static_cast<int32_t>(trunc(durationInMs / msPerHour));
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
		Time timeValue = Time(std::stoi(timeStamp.substr(0, 4)), std::stoi(timeStamp.substr(5, 6)), std::stoi(timeStamp.substr(8, 9)), std::stoi(timeStamp.substr(11, 12)),
			std::stoi(timeStamp.substr(14, 15)), std::stoi(timeStamp.substr(17, 18)));
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
		std::string theString{ stream.str() };
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
		return std::string{ escaped.str() };
	}

	void spinLock(uint64_t timeInNsToSpinLockFor) {
		uint64_t startTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		uint64_t timePassed{ 0 };
		while (timePassed < timeInNsToSpinLockFor) {
			timePassed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - startTime;
		}
	}

	std::string getCurrentISO8601TimeStamp() {
		std::time_t result		 = std::time(nullptr);
		auto resultTwo			 = std::localtime(&result);
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
		uint64_t startTimeRaw	  = convertTimestampToMsInteger(timeStamp);
		auto currentTime		  = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		uint64_t secondsPerMinute = 60;
		uint64_t secondsPerHour	  = secondsPerMinute * 60;
		uint64_t secondsPerDay	  = secondsPerHour * 24;
		auto targetElapsedTime	  = ((days * secondsPerDay) + (hours * secondsPerHour) + (minutes * secondsPerMinute)) * 1000;
		auto actualElapsedTime	  = currentTime - startTimeRaw;
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
		tm time			 = *std::localtime(&now);
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
		uint64_t timeInMs = (std::stoull(this->id) >> 22) + 1420070400000;
		returnString	  = convertTimeInMsToDateTimeString(timeInMs, timeFormat);
		return returnString;
	}
};
