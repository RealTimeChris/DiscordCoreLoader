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

	GuildMemberData ObjectGenerator::generateGuildMember(const ContIterator::String& guildId, const ContIterator::Vector<RoleData>& roles) {
		GuildMemberData theData{};
		theData.nick = randomizeString(
			randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForStringLength), static_cast<double>(DiscordCoreClient::configData.stdDeviationForStringLength)));
		theData.communicationDisabledUntil = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.premiumSince			   = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.joinedAt				   = "";
		int8_t theAmount				   = randomize8BitInt(0, static_cast<uint8_t>(roles.size()));
		theData.permissions				   = std::to_string(randomize64BitUInt());
		for (int32_t x = 0; x < theAmount; x++) {
			theData.roles.emplace_back(roles[x].id);
		}
		theData.flags = static_cast<GuildMemberFlags>(randomize8BitInt());
		theData.user  = generateUser();
		randomizeId(theData.id);
		theData.guildId = guildId;
		return theData;
	}

	UnavailableGuild ObjectGenerator::generateUnavailableGuild(uint64_t minValue, uint64_t maxValue) {
		UnavailableGuild theData{};
		randomizeId(theData.id, minValue, maxValue);
		return theData;
	}

	ChannelData ObjectGenerator::generateChannel(Snowflake guildId) {
		ChannelData theData{};
		theData.rtcRegion = randomizeString(
			randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForStringLength), static_cast<double>(DiscordCoreClient::configData.stdDeviationForStringLength)));
		theData.name = randomizeString(
			randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForStringLength), static_cast<double>(DiscordCoreClient::configData.stdDeviationForStringLength)));
		theData.topic = randomizeString(
			randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForStringLength), static_cast<double>(DiscordCoreClient::configData.stdDeviationForStringLength)));
		theData.permissions						= std::to_string(randomize64BitUInt());
		theData.threadMetadata.archiveTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.member.joinTimestamp			= convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.lastPinTimestamp				= convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData.defaultAutoArchiveDuration		= static_cast<int32_t>(randomize8BitInt());
		for (int32_t x = 0; x < randomize8BitInt(0, 25); x++) {
			OverWriteData theDataNew{};
			randomizeId(theDataNew.channelId);
			randomizeId(theDataNew.id);
			theDataNew.type = static_cast<PermissionOverwritesType>(randomize8BitInt(0, 1));
			theData.permissionOverwrites.emplace_back(theDataNew);
		}
		theData.threadMetadata.autoArchiveDuration = randomize32BitUInt();
		for (int32_t x = 0; x < randomize8BitInt(0, 10); x++) {
			UserData theDataNew						  = generateUser();
			theData.recipients[theDataNew.id.operator const uint64_t&()] = theDataNew;
		}
		theData.type					 = static_cast<ChannelType>(randomize8BitInt(0, 24));
		theData.threadMetadata.archived	 = static_cast<bool>(randomize8BitInt(0, 1));
		theData.threadMetadata.invitable = static_cast<bool>(randomize8BitInt(0, 1));
		theData.threadMetadata.locked	 = static_cast<bool>(randomize8BitInt(0, 1));
		theData.videoQualityMode		 = randomize32BitUInt();
		theData.rateLimitPerUser		 = randomize8BitInt();
		theData.member.flags			 = randomize16BitUInt();
		theData.memberCount				 = randomize8BitInt();
		theData.userLimit				 = randomize32BitUInt();
		theData.bitrate					 = randomize16BitUInt();
		theData.position				 = randomize8BitInt();
		randomizeId(theData.lastMessageId);
		randomizeId(theData.applicationId);
		randomizeId(theData.member.userId);
		theData.flags = static_cast<ChannelFlags>(randomize16BitUInt());
		randomizeId(theData.member.id);
		randomizeId(theData.parentId);
		randomizeId(theData.ownerId);
		theData.icon = randomizeIconHash();
		randomizeId(theData.id);
		theData.guildId = guildId;
		return theData;
	}

	GuildData ObjectGenerator::generateGuild(Snowflake guildId) {
		GuildData theData{};
		auto roleCount = randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForRoleCount), static_cast<double>(DiscordCoreClient::configData.stdDeviationForRoleCount));
		ContIterator::Vector<uint64_t> thePositions{};
		for (uint32_t x = 0; x < roleCount; x++) {
			thePositions.emplace_back(x);
		}
		for (uint32_t x = 0; x < roleCount; x++) {
			theData.roles.emplace_back(generateRole(thePositions[x]));
		}
		auto channelCount = randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForChannelCount), static_cast<double>(DiscordCoreClient::configData.stdDeviationForChannelCount));
		for (uint64_t x = 0; x < channelCount; x++) {
			theData.channels.emplace_back(generateChannel(guildId));
		}
		auto memberCount = randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForMemberCount), static_cast<double>(DiscordCoreClient::configData.stdDeviationForMemberCount));
		for (uint64_t x = 0; x < memberCount; x++) {
			theData.members.emplace_back(generateGuildMember(guildId, theData.roles));
		}
		theData.id = guildId;
		/*
		for (int32_t x = 0; x < theData.memberCount; x++) {
			VoiceStateData theDataNew{};
			theDataNew.channelId = randomizeId();
			theDataNew.deaf = true;
			theDataNew.guildId = guildId;
			theDataNew.member = *theData.members[randomize8BitUInt(0, static_cast<uint8_t>(theData.members.size() - 2))];
			theDataNew.mute = true;
			theDataNew.requestToSpeakTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
			theDataNew.selfDeaf = true;
			theDataNew.selfMute = true;
			theDataNew.selfStream = false;
			theDataNew.selfVideo = false;
			theDataNew.sessionId = randomizeId();
			theDataNew.userId = randomizeId();
			theData.voiceStates.insert_or_assign(theDataNew.userId, theDataNew);
		}*/

		return theData;
	}

	RoleData ObjectGenerator::generateRole(uint64_t position) {
		RoleData theData{};
		theData.name = randomizeString(
			randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForStringLength), static_cast<double>(DiscordCoreClient::configData.stdDeviationForStringLength)));
		theData.unicodeEmoji = randomizeString(
			randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForStringLength), static_cast<double>(DiscordCoreClient::configData.stdDeviationForStringLength)));
		theData.permissions			   = std::to_string(randomize64BitUInt(0, static_cast<uint64_t>(Permission::Moderate_Members)));
		theData.tags.premiumSubscriber = static_cast<bool>(randomize8BitInt(0, 1));
		theData.mentionable			   = static_cast<bool>(randomize8BitInt(0, 1));
		theData.managed				   = static_cast<bool>(randomize8BitInt(0, 1));
		theData.hoist				   = static_cast<bool>(randomize8BitInt(0, 1));
		randomizeId(theData.tags.integrationId);
		theData.color = randomize8BitInt();
		theData.flags = static_cast<RoleFlags>(randomize8BitInt());
		randomizeId(theData.tags.botId);
		theData.icon = randomizeIconHash();
		randomizeId(theData.id);
		theData.position = position;
		return theData;
	}

	UserData ObjectGenerator::generateUser() {
		UserData theData{};
		theData.userName = randomizeString(
			randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForStringLength), static_cast<double>(DiscordCoreClient::configData.stdDeviationForStringLength)));
		theData.email = randomizeString(
			randomize64BitUInt(static_cast<double>(DiscordCoreClient::configData.meanForStringLength), static_cast<double>(DiscordCoreClient::configData.stdDeviationForStringLength)));
		theData.discriminator = ContIterator::String(std::to_string(randomize8BitInt()) + std::to_string(randomize8BitInt())).substr(0, 4);
		theData.premiumType	  = static_cast<PremiumType>(randomize8BitInt());
		theData.avatar		  = randomizeIconHash();
		theData.banner		  = randomizeIconHash();
		theData.mfaEnabled	  = static_cast<bool>(randomize8BitInt(0, 1));
		theData.verified	  = static_cast<bool>(randomize8BitInt(0, 1));
		theData.accentColor	  = randomize8BitInt();
		theData.publicFlags	  = randomize8BitInt(0, 255);
		theData.system		  = static_cast<bool>(randomize8BitInt(0, 1));
		theData.bot			  = static_cast<bool>(randomize8BitInt(0, 1));
		theData.flags		  = randomize8BitInt(0, 255);
		theData.locale		  = randomizeString(4);
		randomizeId(theData.id);
		return theData;
	}
}