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
/// FoundationEntities.cpp - Source file for the foundation entities.
/// May 22, 2022
/// https://discord_core_loader.com
/// \file FoundationEntities.cpp

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/JsonSpecializations.hpp>
#include <discordcoreloader/SSLClients.hpp>

namespace discord_core_loader {

	void reportException(const jsonifier::string& currentFunctionName, std::source_location theLocation) {
		try {
			auto currentException = std::current_exception();
			if (currentException) {
				std::rethrow_exception(currentException);
			}
		} catch (const std::exception& e) {
			std::stringstream theStream{};
			theStream << shiftToBrightRed() << "Error Report: \n"
					  << "Caught At: " << currentFunctionName << ", in File: " << theLocation.file_name() << " ("
					  << jsonifier::toString(theLocation.line()) << ":" << jsonifier::toString(theLocation.column()) << ")"
					  << "\nThe Error: \n"
					  << e.what() << reset() << std::endl
					  << std::endl;
			auto returnString = theStream.str();
			std::cout << returnString;
		}
	}

	jsonifier::string getISO8601TimeStamp(const jsonifier::string& year, const jsonifier::string& month, const jsonifier::string& day, const jsonifier::string& hour,
		const jsonifier::string& minute, const jsonifier::string& second) {
		jsonifier::string theTimeStamp{};
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

	jsonifier::string convertTimeInMsToDateTimeString(uint64_t timeInMs, TimeFormat timeFormat) {
		uint64_t timeValue = timeInMs / 1000;
		time_t rawTime(timeValue);
		tm timeInfo = *localtime(&rawTime);
		jsonifier::string timeStamp{};
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

	jsonifier::string convertMsToDurationString(int32_t durationInMs) {
		jsonifier::string newString{};
		int32_t msPerSecond{ 1000 };
		int32_t secondsPerMinute{ 60 };
		int32_t minutesPerHour{ 60 };
		int32_t msPerMinute{ msPerSecond * secondsPerMinute };
		int32_t msPerHour{ msPerMinute * minutesPerHour };
		int32_t hoursLeft = static_cast<int32_t>(trunc(durationInMs / msPerHour));
		int32_t minutesLeft = static_cast<int32_t>(trunc((durationInMs % msPerHour) / msPerMinute));
		int32_t secondsLeft = static_cast<int32_t>(trunc(((durationInMs % msPerHour) % msPerMinute) / msPerSecond));
		if (hoursLeft >= 1) {
			newString += jsonifier::toString(hoursLeft) + " Hours, ";
			newString += jsonifier::toString(minutesLeft) + " Minutes, ";
			newString += jsonifier::toString(secondsLeft) + " Seconds.";
		} else if (minutesLeft >= 1) {
			newString += jsonifier::toString(minutesLeft) + " Minutes, ";
			newString += jsonifier::toString(secondsLeft) + " Seconds.";
		} else {
			newString += jsonifier::toString(secondsLeft) + " Seconds.";
		}
		return newString;
	}

	jsonifier::string convertToLowerCase(const jsonifier::string& stringToConvert) {
		jsonifier::string newString;
		for (auto& value: stringToConvert) {
			if (isupper(static_cast<int8_t>(value))) {
				newString += static_cast<int8_t>(tolower(static_cast<int8_t>(value)));
			} else {
				newString += value;
			}
		}
		return newString;
	}

	uint64_t convertTimestampToMsInteger(const jsonifier::string& timeStamp) {
		Time timeValue = Time(jsonifier::strToInt64(timeStamp.substr(0, 4)), jsonifier::strToInt64(timeStamp.substr(5, 6)), jsonifier::strToInt64(timeStamp.substr(8, 9)),
			jsonifier::strToInt64(timeStamp.substr(11, 12)), jsonifier::strToInt64(timeStamp.substr(14, 15)), jsonifier::strToInt64(timeStamp.substr(17, 18)));
		return timeValue.getTime() * 1000;
	}

	jsonifier::string base64Encode(const jsonifier::string& theString, bool url) {
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

		jsonifier::string returnString{};
		returnString.reserve(len_encoded);

		uint64_t pos = 0;

		while (pos < theString.size()) {
			returnString.pushBack(base64_chars_[(theString[static_cast<uint64_t>(pos + 0)] & 0xfc) >> 2]);

			if (static_cast<uint64_t>(pos + 1) < theString.size()) {
				returnString.pushBack(base64_chars_[((theString[static_cast<uint64_t>(pos + 0)] & 0x03) << 4) +
					((theString[static_cast<uint64_t>(pos + 1)] & 0xf0) >> 4)]);

				if (static_cast<uint64_t>(pos + 2) < theString.size()) {
					returnString.pushBack(base64_chars_[((theString[static_cast<uint64_t>(pos + 1)] & 0x0f) << 2) +
						((theString[static_cast<uint64_t>(pos + 2)] & 0xc0) >> 6)]);
					returnString.pushBack(base64_chars_[theString[static_cast<uint64_t>(pos + 2)] & 0x3f]);
				} else {
					returnString.pushBack(base64_chars_[(theString[static_cast<uint64_t>(pos + 1)] & 0x0f) << 2]);
					returnString.pushBack(trailing_char);
				}
			} else {
				returnString.pushBack(base64_chars_[(theString[static_cast<uint64_t>(pos + 0)] & 0x03) << 4]);
				returnString.pushBack(trailing_char);
				returnString.pushBack(trailing_char);
			}

			pos += 3;
		}

		return returnString;
	}

	jsonifier::string loadFileContents(const jsonifier::string& filePath) {
		std::ifstream file(filePath.operator std::basic_string<char, std::char_traits<char>, std::allocator<char>>(),
			std::ios::in | std::ios::binary);
		std::ostringstream stream{};
		stream << file.rdbuf();
		std::string theString = stream.str();
		return jsonifier::string{ theString };
	}

	jsonifier::string utf8MakeValid(const jsonifier::string& inputString) {
		jsonifier::string returnString{};
		for (auto& value: inputString) {
			if (value >= 128) {
				returnString.pushBack(value - 128);
			} else {
				returnString.pushBack(value);
			}
		}
		return returnString;
	}

	jsonifier::string urlEncode(const jsonifier::string& inputString) {
		std::ostringstream escaped{};
		escaped.fill('0');
		escaped << std::hex;

		for (jsonifier::string::const_iterator x = inputString.begin(), n = inputString.end(); x != n; ++x) {
			jsonifier::string::value_type c = (*x);

			if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
				escaped << c;
				continue;
			}

			escaped << std::uppercase;
			escaped << '%' << std::setw(2) << int32_t(static_cast<int8_t>(c));
			escaped << std::nouppercase;
		}
		return jsonifier::string{ escaped.str() };
	}

	void spinLock(uint64_t timeInNsToSpinLockFor) {
		uint64_t startTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		uint64_t timePassed{ 0 };
		while (timePassed < timeInNsToSpinLockFor) {
			timePassed =
				std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - startTime;
		}
	}

	jsonifier::string getCurrentISO8601TimeStamp() {
		std::time_t result = std::time(nullptr);
		auto resultTwo = std::localtime(&result);
		jsonifier::string resultString =
			getISO8601TimeStamp(jsonifier::toString(resultTwo->tm_year + 1900), jsonifier::toString(resultTwo->tm_mon), jsonifier::toString(resultTwo->tm_mday),
				jsonifier::toString(resultTwo->tm_hour), jsonifier::toString(resultTwo->tm_min), jsonifier::toString(resultTwo->tm_sec));
		return resultString;
	}

	jsonifier::string generateBase64EncodedKey() {
		jsonifier::string returnString{};
		returnString.resize(16);
		std::mt19937_64 randomEngine{ static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()) };
		for (uint32_t x = 0; x < 16; x++) {
			returnString[x] = static_cast<int8_t>((static_cast<float>(randomEngine()) / static_cast<float>(randomEngine.max())) * 255.0f);
		}
		returnString = base64Encode(returnString, false);
		return returnString;
	}

	jsonifier::string shiftToBrightGreen() {
		return jsonifier::string("\033[1;40;92m");
	}

	jsonifier::string shiftToBrightBlue() {
		return jsonifier::string("\033[1;40;96m");
	}

	jsonifier::string shiftToBrightRed() {
		return jsonifier::string("\033[1;40;91m");
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

	jsonifier::string reset() {
		return jsonifier::string("\033[0m");
	}

	std::ostream& operator<<(std::ostream& outputSttream, const jsonifier::string& (*theFunction)( void )) {
		outputSttream << theFunction();
		return outputSttream;
	}

	bool hasTimeElapsed(const jsonifier::string& timeStamp, uint64_t days, uint64_t hours, uint64_t minutes) {
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

	jsonifier::string getFutureISO8601TimeStamp(int32_t minutesToAdd, int32_t hoursToAdd, int32_t daysToAdd, int32_t monthsToAdd, int32_t yearsToAdd) {
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
		int32_t secondsToAdd = (yearsToAdd * secondsPerYear) + (monthsToAdd * secondsPerMonth) + (daysToAdd * secondsPerDay) +
			(hoursToAdd * secondsPerHour) + (minutesToAdd * secondsPerMinute);
		result += secondsToAdd;
		auto resultTwo = std::localtime(&result);
		jsonifier::string resultString{};
		if (resultTwo->tm_isdst) {
			if (resultTwo->tm_hour + 4 >= 24) {
				resultTwo->tm_hour = resultTwo->tm_hour - 24;
				resultTwo->tm_mday++;
			}
			resultString = getISO8601TimeStamp(jsonifier::toString(resultTwo->tm_year + 1900), jsonifier::toString(resultTwo->tm_mon + 1),
				jsonifier::toString(resultTwo->tm_mday), jsonifier::toString(resultTwo->tm_hour + 4), jsonifier::toString(resultTwo->tm_min),
				jsonifier::toString(resultTwo->tm_sec));
		} else {
			if (resultTwo->tm_hour + 5 >= 24) {
				resultTwo->tm_hour = resultTwo->tm_hour - 24;
				resultTwo->tm_mday++;
			}
			resultString = getISO8601TimeStamp(jsonifier::toString(resultTwo->tm_year + 1900), jsonifier::toString(resultTwo->tm_mon + 1),
				jsonifier::toString(resultTwo->tm_mday), jsonifier::toString(resultTwo->tm_hour + 5), jsonifier::toString(resultTwo->tm_min),
				jsonifier::toString(resultTwo->tm_sec));
		}
		return resultString;
	}

	jsonifier::string getTimeAndDate() {
		const time_t now = std::time(nullptr);
		tm time = *std::localtime(&now);
		jsonifier::string timeStamp{};
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

	jsonifier::string DiscordEntity::getCreatedAtTimestamp(TimeFormat timeFormat) {
		jsonifier::string returnString{};
		uint64_t timeInMs = (jsonifier::strToUint64(this->id) >> 22) + 1420070400000;
		returnString = convertTimeInMsToDateTimeString(timeInMs, timeFormat);
		return returnString;
	}
};
