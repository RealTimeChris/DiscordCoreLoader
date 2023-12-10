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
	If not, see <https:www.gnu.org/licenses/>.

*/
// ObjectGenerator.cpp - Source file for the ObjectGenerator stuff.
// May 22, 2022
// https:discord_core_loader.com
// \file ObjectGenerator.cpp

#include <discordcoreloader/ObjectGenerator.hpp>
#include <discordcoreloader/DiscordCoreClient.hpp>

namespace discord_core_loader {

	std::unique_ptr<GuildMemberData> ObjectGenerator::generateGuildMember(const jsonifier::string& guildId,
		const jsonifier::vector<std::unique_ptr<RoleData>>& roles) {
		std::unique_ptr<GuildMemberData> theData{ std::make_unique<GuildMemberData>() };
		theData->nick = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->communicationDisabledUntil = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData->premiumSince = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData->joinedAt = "";
		int8_t theAmount = this->randomize8BitInt(0, static_cast<uint8_t>(roles.size()));
		theData->permissions = jsonifier::toString(this->randomize64BitUInt());
		theData->pending = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->deaf = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->mute = static_cast<bool>(this->randomize8BitInt(0, 1));
		for (int32_t x = 0; x < theAmount; x++) {
			theData->roles.emplace_back(roles[x]->id);
		}
		theData->flags = this->randomize8BitInt();
		theData->avatar = this->randomizeIconHash();
		theData->user = *this->generateUser();
		this->randomizeId(theData->id);
		theData->guildId = guildId;
		return theData;
	}

	UnavailableGuild ObjectGenerator::generateUnavailableGuild(uint64_t minValue, uint64_t maxValue) {
		UnavailableGuild theData{};
		this->randomizeId(theData.id, minValue, maxValue);
		return theData;
	}

	std::unique_ptr<ChannelData> ObjectGenerator::generateChannel(jsonifier::string guildId) {
		std::unique_ptr<ChannelData> theData{ std::make_unique<ChannelData>() };
		theData->rtcRegion = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->name = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->topic = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->permissions = jsonifier::toString(this->randomize64BitUInt());
		theData->threadMetadata.archiveTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData->member.joinTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData->lastPinTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
		theData->defaultAutoArchiveDuration = static_cast<int32_t>(this->randomize8BitInt());
		for (int32_t x = 0; x < this->randomize8BitInt(0, 25); x++) {
			OverWriteData theDataNew{};
			this->randomizeId(theDataNew.channelId);
			this->randomizeId(theDataNew.id);
			theDataNew.type = static_cast<PermissionOverwritesType>(this->randomize8BitInt(0, 1));
			theData->permissionOverwrites[theDataNew.id] = theDataNew;
		}
		theData->threadMetadata.autoArchiveDuration = this->randomize32BitUInt();
		for (int32_t x = 0; x < this->randomize8BitInt(0, 10); x++) {
			UserData theDataNew = *this->generateUser();
			theData->recipients[theDataNew.id] = theDataNew;
		}
		theData->type = static_cast<ChannelType>(this->randomize8BitInt(0, 24));
		theData->threadMetadata.archived = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->threadMetadata.invitable = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->threadMetadata.locked = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->videoQualityMode = this->randomize32BitUInt();
		theData->rateLimitPerUser = this->randomize8BitInt();
		theData->member.flags = this->randomize16BitUInt();
		theData->memberCount = this->randomize8BitInt();
		theData->userLimit = this->randomize32BitUInt();
		theData->bitrate = this->randomize16BitUInt();
		theData->position = this->randomize8BitInt();
		this->randomizeId(theData->lastMessageId);
		this->randomizeId(theData->applicationId);
		this->randomizeId(theData->member.userId);
		theData->flags = this->randomize16BitUInt();
		this->randomizeId(theData->member.id);
		this->randomizeId(theData->parentId);
		this->randomizeId(theData->ownerId);
		theData->icon = this->randomizeIconHash();
		this->randomizeId(theData->id);
		theData->guildId = guildId;
		return theData;
	}

	std::unique_ptr<GuildData> ObjectGenerator::generateGuild(jsonifier::string guildId) {
		std::unique_ptr<GuildData> theData{ std::make_unique<GuildData>() };
		auto roleCount = this->randomize64BitUInt(static_cast<double>(this->meanForRoleCount), static_cast<double>(this->stdDeviationForRoleCount));
		jsonifier::vector<uint64_t> thePositions{};
		for (uint32_t x = 0; x < roleCount; x++) {
			thePositions.emplace_back(x);
		}
		for (uint32_t x = 0; x < roleCount; x++) {
			theData->roles.emplace_back(this->generateRole(thePositions[x]));
		}
		auto channelCount =
			this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount));
		for (uint64_t x = 0; x < channelCount; x++) {
			theData->channels.emplace_back(this->generateChannel(guildId));
		}
		theData->memberCount =
			this->randomize64BitUInt(static_cast<double>(this->meanForMemberCount), static_cast<double>(this->stdDeviationForMemberCount));
		for (uint64_t x = 0; x < theData->memberCount; x++) {
			theData->members.emplace_back(this->generateGuildMember(guildId, theData->roles));
		}

		theData->defaultMessageNotifications = static_cast<DefaultMessageNotificationLevel>(this->randomize8BitInt());
		theData->afkTimeOut = static_cast<AfkTimeOutDurations>(this->randomize16BitUInt());
		theData->approximatePresenceCount = this->randomize32BitUInt();
		theData->approximateMemberCount = this->randomize32BitUInt();
		theData->banner = this->randomizeIconHash();
		this->randomizeId(theData->applicationId);
		this->randomizeId(theData->afkChannelId);
		theData->description = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->discoverySplash = this->randomizeIconHash();
		theData->explicitContentFilter = static_cast<ExplicitContentFilterLevel>(this->randomize8BitUInt());
		for (int32_t x = 0; x < this->randomize8BitUInt() % 25; x++) {
			theData->features.emplace_back(this->randomizeString(
				this->randomize64BitUInt(static_cast<double>(this->meanForChannelCount), static_cast<double>(this->stdDeviationForChannelCount))));
		}
		theData->flags = this->randomize8BitUInt();
		theData->icon = this->randomizeIconHash();
		theData->iconHash = this->randomizeIconHash();
		theData->id = guildId;
		theData->joinedAt = "";
		theData->maxMembers = this->randomize32BitUInt();
		theData->maxPresences = this->randomize32BitUInt();
		theData->maxVideoChannelUsers = this->randomize32BitUInt();
		theData->mfaLevel = static_cast<MFALevel>(this->randomize8BitUInt());
		theData->name = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->nsfwLevel = GuildNSFWLevel{ this->randomize8BitUInt() };
		this->randomizeId(theData->ownerId);
		theData->permissions = jsonifier::toString(this->randomize64BitUInt());
		theData->preferredLocale = this->randomizeString(4);
		theData->premiumSubscriptionCount = this->randomize32BitUInt();
		theData->premiumTier = PremiumTier{ this->randomize8BitUInt() };
		for (int32_t x = 0; x < theData->memberCount; x++) {
			PresenceUpdateData theDataNew{};
			ClientStatusData theDataNewer{};
			theDataNewer.desktop = this->randomizeString(
				this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
			theDataNewer.mobile = this->randomizeString(
				this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
			theDataNewer.web = this->randomizeString(
				this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
			theDataNew.clientStatus = theDataNewer;
			theDataNew.guildId = guildId;
			theDataNew.status = this->randomizeString(
				this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
			theDataNew.user = *this->generateUser();
			theData->presences[theDataNew.user.id] = theDataNew;
		}
		this->randomizeId(theData->publicUpdatesChannelId);
		theData->region = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		this->randomizeId(theData->rulesChannelId);
		theData->splash = this->randomizeIconHash();
		theData->systemChannelFlags = SystemChannelFlags{ this->randomize8BitUInt() };
		this->randomizeId(theData->systemChannelId);
		theData->vanityUrlCode = this->randomizeString(5);
		theData->verificationLevel = VerificationLevel{ this->randomize8BitUInt() };
		/*
		for (int32_t x = 0; x < theData->memberCount; x++) {
			VoiceStateData theDataNew{};
			theDataNew.channelId = this->randomizeId();
			theDataNew.deaf = true;
			theDataNew.guildId = guildId;
			theDataNew.member = *theData->members[this->randomize8BitUInt(0, static_cast<uint8_t>(theData->members.size() - 2))];
			theDataNew.mute = true;
			theDataNew.requestToSpeakTimestamp = convertTimeInMsToDateTimeString(time(nullptr), TimeFormat::LongDateTime);
			theDataNew.selfDeaf = true;
			theDataNew.selfMute = true;
			theDataNew.selfStream = false;
			theDataNew.selfVideo = false;
			theDataNew.sessionId = this->randomizeId();
			theDataNew.userId = this->randomizeId();
			theData->voiceStates.insert_or_assign(theDataNew.userId, theDataNew);
		}*/

		WelcomeScreenData theDataNew{};
		theDataNew.description = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->welcomeScreen = theDataNew;
		this->randomizeId(theData->widgetChannelId);

		return theData;
	}

	std::unique_ptr<RoleData> ObjectGenerator::generateRole(uint64_t position) {
		std::unique_ptr<RoleData> theData{ std::make_unique<RoleData>() };
		theData->name = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->unicodeEmoji = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->permissions = jsonifier::toString(this->randomize64BitUInt(0, static_cast<uint64_t>(Permission::Moderate_Members)));
		theData->tags.premiumSubscriber = static_cast<jsonifier::string>(this->randomizeString(5));
		theData->mentionable = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->managed = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->hoist = static_cast<bool>(this->randomize8BitInt(0, 1));
		this->randomizeId(theData->tags.integrationId);
		theData->color = this->randomize8BitInt();
		theData->flags = this->randomize8BitInt();
		this->randomizeId(theData->tags.botId);
		theData->icon = this->randomizeIconHash();
		this->randomizeId(theData->id);
		theData->position = position;
		return theData;
	}

	std::unique_ptr<UserData> ObjectGenerator::generateUser() {
		std::unique_ptr<UserData> theData{ std::make_unique<UserData>() };
		theData->userName = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->email = this->randomizeString(
			this->randomize64BitUInt(static_cast<double>(this->meanForStringLength), static_cast<double>(this->stdDeviationForStringLength)));
		theData->discriminator = jsonifier::string(jsonifier::toString(this->randomize8BitInt()) + jsonifier::toString(this->randomize8BitInt())).substr(0, 4);
		theData->premiumType = static_cast<PremiumType>(this->randomize8BitInt());
		theData->avatar = this->randomizeIconHash();
		theData->banner = this->randomizeIconHash();
		theData->mfaEnabled = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->verified = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->accentColor = this->randomize8BitInt();
		theData->publicFlags = randomize8BitInt(0, 255);
		theData->system = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->bot = static_cast<bool>(this->randomize8BitInt(0, 1));
		theData->flags = randomize8BitInt(0, 255);
		theData->locale = this->randomizeString(4);
		this->randomizeId(theData->id);
		return theData;
	}
}