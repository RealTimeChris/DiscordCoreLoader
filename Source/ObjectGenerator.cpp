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
/// ObjectGenerator.cpp - Source file for the ObjectGenerator stuff.
/// May 22, 2022
/// https://discordcoreapi.com
/// \file ObjectGenerator.cpp

#include <discordcoreloader/ObjectGenerator.hpp>
#include <discordcoreloader/DiscordCoreClient.hpp>

namespace DiscordCoreLoader {

	ObjectGenerator::ObjectGenerator(double mean, double stddev) : Randomizer(mean, stddev) {}

	GuildMemberData ObjectGenerator::generateGuildMember(const std::string& guildId, const std::vector<RoleData>& roles) {
		GuildMemberData theData{};
		theData.nick = this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.communicationDisabledUntil = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.premiumSince = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.joinedAt = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		int8_t theAmount = this->randomize8BitInt(0, roles.size());
		theData.permissions = this->randomize64BitUInt();
		theData.pending = this->randomize8BitInt(0, 1);
		theData.deaf = this->randomize8BitInt(0, 1);
		theData.mute = this->randomize8BitInt(0, 1);
		for (int32_t x = 0; x < theAmount; x += 1) {
			theData.roles.push_back(roles[x].id);
		}
		theData.flags = this->randomize8BitInt();
		theData.avatar = this->randomizeId();
		theData.user = this->generateUser();
		theData.id = this->randomizeId();
		theData.guildId = guildId;
		return theData;
	}

	UnavailableGuild ObjectGenerator::generateUnavailableGuild(uint64_t minValue, uint64_t maxValue) {
		UnavailableGuild theData{};
		theData.id = this->randomizeId(minValue, maxValue);
		return theData;
	}

	ChannelData ObjectGenerator::generateChannel(std::string guildId) {
		ChannelData theData{};
		theData.rtcRegion = this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.name = this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.topic = this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.permissions =
			this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.threadMetadata.archiveTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.member.joinTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.lastPinTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.defaultAutoArchiveDuration = static_cast<int32_t>(this->randomize8BitInt());
		for (int32_t x = 0; x < this->randomize8BitInt(0, 25); x += 1) {
			OverWriteData theDataNew{};
			theDataNew.channelId = this->randomizeId();
			theDataNew.id = this->randomizeId();
			theDataNew.type = static_cast<PermissionOverwritesType>(this->randomize8BitInt(0, 1));
			theData.permissionOverwrites.insert_or_assign(theDataNew.id, theDataNew);
		}
		theData.threadMetadata.autoArchiveDuration = this->randomize32BitUInt();
		for (int32_t x = 0; x < this->randomize8BitInt(0, 10); x += 1) {
			UserData theDataNew = this->generateUser();
			theData.recipients.insert_or_assign(theDataNew.id, theDataNew);
		}
		theData.type = static_cast<ChannelType>(this->randomize8BitInt(0, 24));
		theData.threadMetadata.archived = this->randomize8BitInt(0, 1);
		theData.threadMetadata.invitable = this->randomize8BitInt(0, 1);
		theData.threadMetadata.locked = this->randomize8BitInt(0, 1);
		theData.videoQualityMode = this->randomize32BitUInt();
		theData.rateLimitPerUser = this->randomize8BitInt();
		theData.member.flags = this->randomize16BitUInt();
		theData.memberCount = this->randomize8BitInt();
		theData.userLimit = this->randomize32BitUInt();
		theData.bitrate = this->randomize16BitUInt();
		theData.position = this->randomize8BitInt();
		theData.lastMessageId = this->randomizeId();
		theData.applicationId = this->randomizeId();
		theData.member.userId = this->randomizeId();
		theData.flags = this->randomize16BitUInt();
		theData.member.id = this->randomizeId();
		theData.parentId = this->randomizeId();
		theData.ownerId = this->randomizeId();
		theData.icon = this->randomizeId();
		theData.id = this->randomizeId();
		theData.guildId = guildId;
		return theData;
	}

	GuildData ObjectGenerator::generateGuild(std::string guildId) {
		GuildData theData{};
		auto roleCount = this->randomize64BitUInt(static_cast<double>(this->meanForRoleCount), static_cast<double>(this->stdDeviationForRoleCount));
		std::vector<uint64_t> thePositions{};
		for (uint32_t x = 0; x < roleCount; x += 1) {
			thePositions.push_back(x);
		}
		for (uint32_t x = 0; x < roleCount; x += 1) {
			theData.roles.push_back(this->generateRole(thePositions[x]));
		}
		auto channelCount = this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount));
		for (uint64_t x = 0; x < channelCount; x += 1) {
			theData.channels.push_back(this->generateChannel(guildId));
		}
		theData.memberCount = this->randomize64BitUInt(static_cast<double>(this->meanForMemberCount), static_cast<double>(this->stdDeviationForMemberCount));
		for (uint64_t x = 0; x < theData.memberCount; x += 1) {
			theData.members.push_back(this->generateGuildMember(theData.id, theData.roles));
		}
		theData.defaultMessageNotifications = static_cast<DefaultMessageNotificationLevel>(this->randomize8BitInt());
		theData.afkTimeOut = static_cast<AfkTimeOutDurations>(this->randomize16BitUInt());
		theData.approximatePresenceCount = this->randomize32BitUInt();
		theData.approximateMemberCount = this->randomize32BitUInt();
		theData.banner = std::to_string(this->randomize64BitUInt());
		theData.applicationId = this->randomizeId();
		theData.afkChannelId = this->randomizeId();
		return theData;
	}

	RoleData ObjectGenerator::generateRole(uint64_t position) {
		RoleData theData{};
		theData.name = this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.unicodeEmoji =
			this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.permissions = this->randomize64BitUInt(0, static_cast<uint64_t>(Permission::Moderate_Members));
		theData.tags.premiumSubscriber = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.mentionable = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.managed = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.hoist = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.tags.integrationId = this->randomizeId();
		theData.color = this->randomize8BitInt();
		theData.flags = this->randomize8BitInt();
		theData.tags.botId = this->randomizeId();
		theData.icon = this->randomizeId();
		theData.id = this->randomizeId();
		theData.position = position;
		return theData;
	}

	UserData ObjectGenerator::generateUser() {
		UserData theData{};
		theData.userName = this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.email = this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.email = this->randomizeString(this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount)));
		theData.discriminator = std::string(std::to_string(this->randomize8BitInt()) + std::to_string(this->randomize8BitInt())).substr(0, 4);
		theData.premiumType = static_cast<PremiumType>(this->randomize8BitInt());
		theData.avatar = std::to_string(this->randomize64BitUInt());
		theData.banner = std::to_string(this->randomize64BitUInt());
		theData.mfaEnabled = this->randomize8BitInt(0, 1);
		theData.verified = this->randomize8BitInt(0, 1);
		theData.accentColor = this->randomize8BitInt();
		theData.publicFlags = randomize8BitInt(0, 255);
		theData.system = this->randomize8BitInt(0, 1);
		theData.bot = this->randomize8BitInt(0, 1);
		theData.flags = randomize8BitInt(0, 255);		
		theData.locale = this->randomizeString(4);
		theData.id = this->randomizeId();
		return theData;
	}
}