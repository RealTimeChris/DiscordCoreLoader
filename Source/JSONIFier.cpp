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
/// JSONIfier.cpp - Source file for the Jsonifier stuff.
/// May 22, 2022
/// https://discordcoreapi.com
/// \file JSONIfier.cpp

#include <discordcoreloader/JSONIfier.hpp>

namespace DiscordCoreLoader {

	JSONIFier& JSONIFier::operator=(ConfigData&& configData) {
		this->stdDeviationForChannelCount = configData.stdDeviationForChannelCount;
		this->stdDeviationForStringLength = configData.stdDeviationForStringLength;
		this->stdDeviationForMemberCount = configData.stdDeviationForMemberCount;
		this->stdDeviationForRoleCount = configData.stdDeviationForRoleCount;
		this->meanForChannelCount = configData.meanForChannelCount;
		this->meanForStringLength = configData.meanForStringLength;
		this->meanForMemberCount = configData.meanForMemberCount;
		this->meanForRoleCount = configData.meanForRoleCount;
		return *this;
	}

	JSONIFier::JSONIFier(ConfigData&& configData) {
		*this = std::move(configData);
	}

	nlohmann::json JSONIFier::JSONIFYUnavailableGuild(UnavailableGuild& theGuild) {
		nlohmann::json jsonData{};
		jsonData["id"] = theGuild.id;
		jsonData["unavailable"] = theGuild.unavailable;
		return jsonData;
	}

	nlohmann::json JSONIFier::JSONIFYGuild(GuildData&& guildOld) {
		nlohmann::json jsonData{};
		jsonData["id"] = guildOld.id;
		uint64_t memberCount = guildOld.memberCount;
		jsonData["channels"];
		for (auto& value: guildOld.channels) {
			jsonData["channels"].push_back(this->JSONIFYChannel(std::move(*value)));
		}
		jsonData["member_count"] = memberCount;
		jsonData["members"];
		for (auto& value: guildOld.members) {
			jsonData["members"].push_back(this->JSONIFYGuildMember(std::move(*value)));
		}
		jsonData["roles"];
		for (auto& value: guildOld.roles) {
			jsonData["roles"].push_back(this->JSONIFYRole(std::move(*value)));
		}
		jsonData["afk_channel_id"] = guildOld.afkChannelId;
		jsonData["afk_time_out"] = guildOld.afkTimeOut;
		jsonData["application_id"] = guildOld.applicationId;
		jsonData["approximate_member_count"] = guildOld.approximateMemberCount;
		jsonData["approximate_presence_count"] = guildOld.approximatePresenceCount;
		jsonData["banner"] = guildOld.banner;
		jsonData["default_message_notifications"] = guildOld.defaultMessageNotifications;
		jsonData["description"] = guildOld.description;
		jsonData["discovery_splash"] = guildOld.discoverySplash;
		jsonData["explicit_content_filter"] = guildOld.explicitContentFilter;
		for (auto& value: guildOld.features) {
			jsonData["features"].push_back(value);
		}
		jsonData["flags"] = guildOld.flags;
		jsonData["icon"] = guildOld.icon;
		jsonData["icon_hash"] = guildOld.iconHash;
		jsonData["joined_at"] = guildOld.joinedAt.getOriginalTimeStamp();
		jsonData["max_members"] = guildOld.maxMembers;
		jsonData["max_presences"] = guildOld.maxPresences;
		jsonData["max_video_channel_users"] = guildOld.maxVideoChannelUsers;
		jsonData["member_count"] = guildOld.memberCount;
		jsonData["mfa_level"] = guildOld.mfaLevel;
		jsonData["name"] = guildOld.name;
		jsonData["nsfw_level"] = guildOld.nsfwLevel;
		jsonData["owner_id"] = guildOld.ownerId;
		jsonData["permissions"] = guildOld.permissions;
		jsonData["preferred_locale"] = guildOld.preferredLocale;
		jsonData["premium_subscription_count"] = guildOld.premiumSubscriptionCount;
		jsonData["premium_tier"] = guildOld.premiumTier;
		for (auto& [key, value]: guildOld.presences) {
			nlohmann::json theDataNew{};
			theDataNew["guild_id"] = value.guildId;
			theDataNew["status"] = value.status;
			theDataNew["user"] = this->JSONIFYUser(std::move(value.user));
			theDataNew["client_status"]["desktop"] = value.clientStatus.desktop;
			theDataNew["client_status"]["mobile"] = value.clientStatus.mobile;
			theDataNew["client_status"]["web"] = value.clientStatus.web;
			jsonData["presences"].push_back(theDataNew);
		}
		jsonData["public_updates_channel_id"] = guildOld.publicUpdatesChannelId;
		jsonData["region"] = guildOld.region;
		jsonData["rules_channel_id"] = guildOld.rulesChannelId;
		jsonData["splash"] = guildOld.splash;
		jsonData["system_channel_flags"] = guildOld.systemChannelFlags;
		jsonData["system_channel_id"] = guildOld.systemChannelId;
		jsonData["vanity_url_code"] = guildOld.vanityUrlCode;
		jsonData["verification_level"] = guildOld.verificationLevel;
		jsonData["welcome_screen"]["description"] = guildOld.welcomeScreen.description;
		for (auto& [key, value]: guildOld.voiceStates) {
			nlohmann::json theDataNew{};
			theDataNew["channel_id"] = value.channelId;
			theDataNew["deaf"] = value.deaf;
			theDataNew["guild_id"] = value.guildId;
			theDataNew["member"] = this->JSONIFYGuildMember(std::move(value.member));
			theDataNew["mute"] = value.mute;
			theDataNew["request_to_speak_timestamp"] = value.requestToSpeakTimestamp.getOriginalTimeStamp();
			theDataNew["self_deaf"] = value.selfDeaf;
			theDataNew["self_mute"] = value.selfMute;
			theDataNew["self_stream"] = value.selfStream;
			theDataNew["self_video"] = value.selfVideo;
			theDataNew["session_id"] = value.sessionId;
			theDataNew["suppress"] = value.suppress;
			theDataNew["user_id"] = value.userId;
			jsonData["voice_states"].push_back(theDataNew);
		}
		jsonData["widget_channel_id"] = guildOld.widgetChannelId;
		return jsonData;
	}

	nlohmann::json JSONIFier::JSONIFYGuildMember(GuildMemberData&& theData) {
		nlohmann::json jsonData{};
		jsonData["communication_disabled_until"] = theData.communicationDisabledUntil.getOriginalTimeStamp();
		jsonData["user"] = this->JSONIFYUser(std::move(*this->generateUser()));
		jsonData["joined_at"] = theData.joinedAt.getOriginalTimeStamp();
		jsonData["premium_since"] = theData.premiumSince;
		jsonData["permissions"] = theData.permissions;
		jsonData["guild_id"] = theData.guildId;
		for (auto& value: theData.roles) {
			jsonData["roles"].push_back(value);
		}
		jsonData["pending"] = theData.pending;
		jsonData["avatar"] = theData.avatar;
		jsonData["flags"] = theData.flags;
		jsonData["nick"] = theData.nick;
		jsonData["deaf"] = theData.deaf;
		jsonData["mute"] = theData.mute;
		return jsonData;
	}

	nlohmann::json JSONIFier::JSONIFYChannel(ChannelData&& theData) {
		nlohmann::json jsonData{};
		jsonData["thread_metadata"]["auto_archive_duration"] = theData.threadMetadata.autoArchiveDuration;
		for (auto& [key, value]: theData.permissionOverwrites) {
			nlohmann::json dataNew{};
			dataNew["id"] = value.id;
			dataNew["channel_id"] = value.channelId;
			dataNew["type"] = value.type;
			jsonData["permission_overwrites"].push_back(dataNew);
		}
		jsonData["thread_metadata"]["archive_timestamp"] = theData.threadMetadata.archiveTimestamp.getOriginalTimeStamp();
		for (auto& [key, value]: theData.recipients) {
			jsonData["recipients"].push_back(this->JSONIFYUser(std::move(value)));
		}
		jsonData["member"]["join_timestamp"] = theData.member.joinTimestamp.getOriginalTimeStamp();
		jsonData["last_pin_timestamp"] = theData.lastPinTimestamp.getOriginalTimeStamp();
		jsonData["default_auto_archive_duration"] = theData.defaultAutoArchiveDuration;
		jsonData["thread_metadata"]["invitable"] = theData.threadMetadata.invitable;
		jsonData["thread_metadata"]["archived"] = theData.threadMetadata.archived;
		jsonData["thread_metadata"]["locked"] = theData.threadMetadata.locked;
		jsonData["rate_limit_per_user"] = theData.rateLimitPerUser;
		jsonData["video_quality_mode"] = theData.videoQualityMode;
		jsonData["member"]["user_id"] = theData.member.userId;
		jsonData["last_message_id"] = theData.lastMessageId;
		jsonData["member"]["flags"] = theData.member.flags;
		jsonData["application_id"] = theData.applicationId;
		jsonData["message_count"] = theData.messageCount;
		jsonData["member_count"] = theData.memberCount;
		jsonData["permissions"] = theData.permissions;
		jsonData["permissions"] = theData.permissions;
		jsonData["member"]["id"] = theData.member.id;
		jsonData["rtc_region"] = theData.rtcRegion;
		jsonData["user_limit"] = theData.userLimit;
		jsonData["parent_id"] = theData.parentId;
		jsonData["position"] = theData.position;
		jsonData["position"] = theData.position;
		jsonData["owner_id"] = theData.ownerId;
		jsonData["guild_id"] = theData.guildId;
		jsonData["bitrate"] = theData.bitrate;
		jsonData["flags"] = theData.flags;
		jsonData["topic"] = theData.topic;
		jsonData["type"] = theData.type;
		jsonData["icon"] = theData.icon;
		jsonData["name"] = theData.name;
		jsonData["name"] = theData.name;
		jsonData["id"] = theData.id;
		return jsonData;
	}

	nlohmann::json JSONIFier::JSONIFYRole(RoleData&& theData) {
		nlohmann::json jsonData{};
		jsonData["tags"]["premium_subscriber"] = theData.tags.premiumSubscriber;
		jsonData["tags"]["integration_id"] = theData.tags.integrationId;
		jsonData["unicode_emoji"] = theData.unicodeEmoji;
		jsonData["tags"]["bot_id"] = theData.tags.botId;
		jsonData["mentionable"] = theData.mentionable;
		jsonData["permissions"] = theData.permissions;
		jsonData["position"] = theData.position;
		jsonData["managed"] = theData.managed;
		jsonData["hoist"] = theData.hoist;
		jsonData["flags"] = theData.flags;
		jsonData["color"] = theData.color;
		jsonData["icon"] = theData.icon;
		jsonData["name"] = theData.name;
		jsonData["id"] = theData.id;
		return jsonData;
	}

	nlohmann::json JSONIFier::JSONIFYUser(UserData&& theData) {
		nlohmann::json jsonData{};
		jsonData["discriminator"] = theData.discriminator;
		jsonData["accent_color"] = theData.accentColor;
		jsonData["premium_type"] = theData.premiumType;
		jsonData["public_flags"] = theData.publicFlags;
		jsonData["mfa_enabled"] = theData.mfaEnabled;
		jsonData["username"] = theData.userName;
		jsonData["verified"] = theData.verified;
		jsonData["avatar"] = theData.avatar;
		jsonData["banner"] = theData.banner;
		jsonData["locale"] = theData.locale;
		jsonData["system"] = theData.system;
		jsonData["flags"] = theData.flags;
		jsonData["email"] = theData.email;
		jsonData["bot"] = theData.bot;
		jsonData["id"] = theData.id;
		return jsonData;
	}

}