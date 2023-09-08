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
	If not, see <https:www.gnu.org/licenses/>.

*/
// ObjectGenerator.cpp - Source file for the ObjectGenerator stuff.
// May 22, 2022
// https:discordcoreloader.com
// \file ObjectGenerator.cpp

#include <discordcoreloader/ObjectGenerator.hpp>
#include <discordcoreloader/DiscordCoreClient.hpp>

namespace DiscordCoreLoader {

	ObjectGenerator& ObjectGenerator::operator=(const ConfigData& configDataNew) {
		configData = configDataNew;
		return *this;
	}

	ObjectGenerator::ObjectGenerator(const ConfigData& configData) {
		*this = configData;
	}

	GuildMemberData ObjectGenerator::generateGuildMember(const std::string& guildId, const std::vector<RoleData>& roles) {
		GuildMemberData theData{};
		theData.nick = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->configData.meanForStringLength), static_cast<double>(this->configData.stdDeviationForStringLength)));
		theData.communicationDisabledUntil = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.premiumSince			   = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.joinedAt				   = "";
		int8_t theAmount				   = this->randomize8BitInt(0, static_cast<uint8_t>(roles.size()));
		theData.permissions				   = std::to_string(this->randomize64BitUInt());
		for (int32_t x = 0; x < theAmount; x++) {
			theData.roles.emplace_back(roles[x].id);
		}
		theData.flags = static_cast<GuildMemberFlags>(this->randomize8BitInt());
		theData.user  = this->generateUser();
		this->randomizeId(theData.id);
		theData.guildId = guildId;
		return theData;
	}

	UnavailableGuild ObjectGenerator::generateUnavailableGuild(uint64_t minValue, uint64_t maxValue) {
		UnavailableGuild theData{};
		this->randomizeId(theData.id, minValue, maxValue);
		return theData;
	}

	ChannelData ObjectGenerator::generateChannel(std::string guildId) {
		ChannelData theData{};
		theData.rtcRegion = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->configData.meanForStringLength), static_cast<double>(this->configData.stdDeviationForStringLength)));
		theData.name = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->configData.meanForStringLength), static_cast<double>(this->configData.stdDeviationForStringLength)));
		theData.topic = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->configData.meanForStringLength), static_cast<double>(this->configData.stdDeviationForStringLength)));
		theData.permissions						= std::to_string(this->randomize64BitUInt());
		theData.threadMetadata.archiveTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.member.joinTimestamp			= convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.lastPinTimestamp				= convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.defaultAutoArchiveDuration		= static_cast<int32_t>(this->randomize8BitInt());
		for (int32_t x = 0; x < this->randomize8BitInt(0, 25); x++) {
			OverWriteData theDataNew{};
			this->randomizeId(theDataNew.channelId);
			this->randomizeId(theDataNew.id);
			theDataNew.type = static_cast<PermissionOverwritesType>(this->randomize8BitInt(0, 1));
			theData.permissionOverwrites.emplace_back(theDataNew);
		}
		theData.threadMetadata.autoArchiveDuration = this->randomize32BitUInt();
		for (int32_t x = 0; x < this->randomize8BitInt(0, 10); x++) {
			UserData theDataNew						  = this->generateUser();
			theData.recipients[std::stoull(theDataNew.id)] = theDataNew;
		}
		theData.type					 = static_cast<ChannelType>(this->randomize8BitInt(0, 24));
		theData.threadMetadata.archived	 = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.threadMetadata.invitable = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.threadMetadata.locked	 = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.videoQualityMode		 = this->randomize32BitUInt();
		theData.rateLimitPerUser		 = this->randomize8BitInt();
		theData.member.flags			 = this->randomize16BitUInt();
		theData.memberCount				 = this->randomize8BitInt();
		theData.userLimit				 = this->randomize32BitUInt();
		theData.bitrate					 = this->randomize16BitUInt();
		theData.position				 = this->randomize8BitInt();
		this->randomizeId(theData.lastMessageId);
		this->randomizeId(theData.applicationId);
		this->randomizeId(theData.member.userId);
		theData.flags = static_cast<ChannelFlags>(this->randomize16BitUInt());
		this->randomizeId(theData.member.id);
		this->randomizeId(theData.parentId);
		this->randomizeId(theData.ownerId);
		theData.icon = this->randomizeIconHash();
		this->randomizeId(theData.id);
		theData.guildId = guildId;
		return theData;
	}

	GuildData ObjectGenerator::generateGuild(std::string guildId) {
		GuildData theData{};
		auto roleCount = this->randomize64BitUInt(static_cast<double>(this->configData.meanForRoleCount), static_cast<double>(this->configData.stdDeviationForRoleCount));
		std::vector<uint64_t> thePositions{};
		for (uint32_t x = 0; x < roleCount; x++) {
			thePositions.emplace_back(x);
		}
		for (uint32_t x = 0; x < roleCount; x++) {
			theData.roles.emplace_back(this->generateRole(thePositions[x]));
		}
		auto channelCount = this->randomize64BitUInt(static_cast<double>(this->configData.meanForChannelCount), static_cast<double>(this->configData.stdDeviationForChannelCount));
		for (uint64_t x = 0; x < channelCount; x++) {
			theData.channels.emplace_back(this->generateChannel(guildId));
		}
		auto memberCount = this->randomize64BitUInt(static_cast<double>(this->configData.meanForMemberCount), static_cast<double>(this->configData.stdDeviationForMemberCount));
		for (uint64_t x = 0; x < memberCount; x++) {
			theData.members.emplace_back(this->generateGuildMember(guildId, theData.roles));
		}
		theData.id = guildId;
		/*
		for (int32_t x = 0; x < theData.memberCount; x++) {
			VoiceStateData theDataNew{};
			theDataNew.channelId = this->randomizeId();
			theDataNew.deaf = true;
			theDataNew.guildId = guildId;
			theDataNew.member = *theData.members[this->randomize8BitUInt(0, static_cast<uint8_t>(theData.members.size() - 2))];
			theDataNew.mute = true;
			theDataNew.requestToSpeakTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
			theDataNew.selfDeaf = true;
			theDataNew.selfMute = true;
			theDataNew.selfStream = false;
			theDataNew.selfVideo = false;
			theDataNew.sessionId = this->randomizeId();
			theDataNew.userId = this->randomizeId();
			theData.voiceStates.insert_or_assign(theDataNew.userId, theDataNew);
		}*/

		return theData;
	}

	RoleData ObjectGenerator::generateRole(uint64_t position) {
		RoleData theData{};
		theData.name = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->configData.meanForStringLength), static_cast<double>(this->configData.stdDeviationForStringLength)));
		theData.unicodeEmoji = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->configData.meanForStringLength), static_cast<double>(this->configData.stdDeviationForStringLength)));
		theData.permissions			   = std::to_string(this->randomize64BitUInt(0, static_cast<uint64_t>(Permission::Moderate_Members)));
		theData.tags.premiumSubscriber = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.mentionable			   = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.managed				   = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.hoist				   = static_cast<bool>(this->randomize8BitInt(0, 1));
		this->randomizeId(theData.tags.integrationId);
		theData.color = this->randomize8BitInt();
		theData.flags = static_cast<RoleFlags>(this->randomize8BitInt());
		this->randomizeId(theData.tags.botId);
		theData.icon = this->randomizeIconHash();
		this->randomizeId(theData.id);
		theData.position = position;
		return theData;
	}

	UserData ObjectGenerator::generateUser() {
		UserData theData{};
		theData.userName = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->configData.meanForStringLength), static_cast<double>(this->configData.stdDeviationForStringLength)));
		theData.email = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->configData.meanForStringLength), static_cast<double>(this->configData.stdDeviationForStringLength)));
		theData.discriminator = std::string(std::to_string(this->randomize8BitInt()) + std::to_string(this->randomize8BitInt())).substr(0, 4);
		theData.premiumType	  = static_cast<PremiumType>(this->randomize8BitInt());
		theData.avatar		  = this->randomizeIconHash();
		theData.banner		  = this->randomizeIconHash();
		theData.mfaEnabled	  = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.verified	  = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.accentColor	  = this->randomize8BitInt();
		theData.publicFlags	  = randomize8BitInt(0, 255);
		theData.system		  = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.bot			  = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData.flags		  = randomize8BitInt(0, 255);
		theData.locale		  = this->randomizeString(4);
		this->randomizeId(theData.id);
		return theData;
	}
}