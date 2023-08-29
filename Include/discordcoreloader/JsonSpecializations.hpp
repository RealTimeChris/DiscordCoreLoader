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
/// https://github.com/RealTimeChris/DiscordCoreLoader
/// \file FoundationEntities.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/SSLClients.hpp>

namespace JsonifierInternal {

	template<> inline void ParseWithKeys::op<true, DiscordCoreLoader::TimeStamp>(DiscordCoreLoader::TimeStamp& value, StructuralIterator& it) {
		std::string newString{};
		ParseWithKeys::op<true>(newString, it);
		value = newString;
	}

	template<> inline void SerializeNoKeys::op(DiscordCoreLoader::TimeStamp& value, Jsonifier::String& buffer, uint64_t& index) {
		std::string newString{ static_cast<std::string>(value) };
		SerializeNoKeys::op(newString, buffer, index);
	}
}

namespace Jsonifier {

	template<> struct Core<DiscordCoreLoader::ApplicationCommandPermissionData> {
		using ValueType					 = DiscordCoreLoader::ApplicationCommandPermissionData;
		constexpr static auto parseValue = object("id", &ValueType::id, "permission", &ValueType::permission, "type", &ValueType::type);
	};

	template<> struct Core<DiscordCoreLoader::GuildApplicationCommandPermissionsData> {
		using ValueType = DiscordCoreLoader::GuildApplicationCommandPermissionsData;
		constexpr static auto parseValue =
			object("application_id", &ValueType::applicationId, "permissions", &ValueType::permissions, "guild_id", &ValueType::guildId, "id", &ValueType::id);
	};

	template<> struct Core<DiscordCoreLoader::SessionStartData> {
		using ValueType = DiscordCoreLoader::SessionStartData;
		constexpr static auto parseValue =
			object("max_concurrency", &ValueType::maxConcurrency, "remaining", &ValueType::remaining, "reset_after", &ValueType::resetAfter, "total", &ValueType::total);
	};

	template<> struct Core<DiscordCoreLoader::AuthorizationInfoData> {
		using ValueType					 = DiscordCoreLoader::AuthorizationInfoData;
		constexpr static auto parseValue = object("application", &ValueType::application, "expires", &ValueType::expires, "scopes", &ValueType::scopes, "user", &ValueType::user);
	};

	template<> struct Core<DiscordCoreLoader::EmojiData> {
		using ValueType					 = DiscordCoreLoader::EmojiData;
		constexpr static auto parseValue = object("id", &ValueType::id, "name", &ValueType::name, "roles", &ValueType::roles, "require_colons", &ValueType::requireColons,
			"managed", &ValueType::managed, "animated", &ValueType::animated, "available", &ValueType::available);
	};

	template<> struct Core<DiscordCoreLoader::TeamMembersObjectData> {
		using ValueType					 = DiscordCoreLoader::TeamMembersObjectData;
		constexpr static auto parseValue = object("membership_state", &ValueType::membershipState, "team_id", &ValueType::teamId, "user", &ValueType::user);
	};

	template<> struct Core<DiscordCoreLoader::TeamObjectData> {
		using ValueType					 = DiscordCoreLoader::TeamObjectData;
		constexpr static auto parseValue = object("icon", &ValueType::icon, "id", &ValueType::id, "members", &ValueType::members, "owner_user_id", &ValueType::ownerUserId);
	};

	template<> struct Core<DiscordCoreLoader::InstallParamsData> {
		using ValueType					 = DiscordCoreLoader::InstallParamsData;
		constexpr static auto parseValue = object("scopes", &ValueType::scopes, "permissions", &ValueType::permissions);
	};

	template<> struct Core<DiscordCoreLoader::ApplicationData> {
		using ValueType					 = DiscordCoreLoader::ApplicationData;
		constexpr static auto parseValue = object("bot_public", &ValueType::botPublic, "bot_require_code_grant", &ValueType::botRequireCodeGrant, "cover_image",
			&ValueType::coverImage, "description", &ValueType::description, "", "flags", &ValueType::flags, "guild_id", &ValueType::guildId, "icon", &ValueType::icon, "name",
			&ValueType::name, "owner", &ValueType::owner, "params", &ValueType::params, "primary_sku_id", &ValueType::primarySkuId, "privacy_policy_url",
			&ValueType::privacyPolicyUrl, "rpc_origins", &ValueType::rpcOrigins, "slug", &ValueType::slug, "summary", &ValueType::summary, "tags", &ValueType::tags, "team",
			&ValueType::team, "terms_of_service_url", &ValueType::termsOfServiceUrl, "verify_key", &ValueType::verifyKey);
	};

	template<> struct Core<DiscordCoreLoader::ThreadMetadataData> {
		using ValueType					 = DiscordCoreLoader::ThreadMetadataData;
		constexpr static auto parseValue = object("archived", &ValueType::archived, "archived_timestamp", &ValueType::archiveTimestamp, "auto_archive_duration",
			&ValueType::autoArchiveDuration, "invitable", &ValueType::invitable, "locked", &ValueType::locked);
	};

	template<> struct Core<DiscordCoreLoader::GatewayBotData> {
		using ValueType					 = DiscordCoreLoader::GatewayBotData;
		constexpr static auto parseValue = object("session_start_limit", &ValueType::sessionStartLimit, "shards", &ValueType::shards, "url", &ValueType::url);
	};

	template<> struct Core<DiscordCoreLoader::DiscordEntity> {
		using ValueType					 = DiscordCoreLoader::DiscordEntity;
		constexpr static auto parseValue = object("id", &ValueType::id);
	};

	template<> struct Core<DiscordCoreLoader::UserData> {
		using ValueType					 = DiscordCoreLoader::UserData;
		constexpr static auto parseValue = object("mfa_enabled", &ValueType::flags, "verified", &ValueType::flags, "system", &ValueType::flags, "bot", &ValueType::flags,
			"public_flags", &ValueType::flags, "username", &ValueType::userName, "discriminator", &ValueType::discriminator, "id", &ValueType::id, "avatar", &ValueType::avatar);
	};

	template<> struct Core<DiscordCoreLoader::WelcomeScreenChannelData> {
		using ValueType = DiscordCoreLoader::WelcomeScreenChannelData;
		constexpr static auto parseValue =
			object("channel_id", &ValueType::channelId, "description", &ValueType::description, "emoji_id", &ValueType::emojiId, "emoji_name", &ValueType::emojiName);
	};

	template<> struct Core<DiscordCoreLoader::WelcomeScreenData> {
		using ValueType					 = DiscordCoreLoader::WelcomeScreenData;
		constexpr static auto parseValue = object("description", &ValueType::description, "welcome_screen_channels", &ValueType::welcomeChannels);
	};

	template<> struct Core<DiscordCoreLoader::GuildPreviewData> {
		using ValueType					 = DiscordCoreLoader::GuildPreviewData;
		constexpr static auto parseValue = object("approximate_member_count", &ValueType::approximateMemberCount, "approximate_presence_count",
			&ValueType::approximatePresenceCount, "description", &ValueType::description, "discovery_splash", &ValueType::discoverySplash, "emojis", &ValueType::emojis, "features",
			&ValueType::features, "icon", &ValueType::icon, "id", &ValueType::id, "name", &ValueType::name, "splash", &ValueType::splash, "stickers", &ValueType::stickers);
	};

	template<> struct Core<DiscordCoreLoader::ThreadMemberData> {
		using ValueType					 = DiscordCoreLoader::ThreadMemberData;
		constexpr static auto parseValue = object("flags", &ValueType::flags, "join_timestamp", &ValueType::joinTimestamp, "user_id", &ValueType::userId);
	};

	template<> struct Core<DiscordCoreLoader::AuditLogData> {
		using ValueType					 = DiscordCoreLoader::AuditLogData;
		constexpr static auto parseValue = object("audit_log_entries", &ValueType::auditLogEntries, "guild_scheduled_events", &ValueType::guildScheduledEvents, "integrations",
			&ValueType::integrations, "threads", &ValueType::threads, "users", &ValueType::users, "webhooks", &ValueType::webhooks);
	};

	template<> struct Core<DiscordCoreLoader::BanData> {
		using ValueType					 = DiscordCoreLoader::BanData;
		constexpr static auto parseValue = object("failed_due_to_perms", &ValueType::failedDueToPerms, "reason", &ValueType::reason, "user", &ValueType::user);
	};

	template<> struct Core<DiscordCoreLoader::OverWriteData> {
		using ValueType					 = DiscordCoreLoader::OverWriteData;
		constexpr static auto parseValue = object("channel_id", &ValueType::channelId, "id", &ValueType::id, "type", &ValueType::type);
	};

	template<> struct Core<DiscordCoreLoader::UpdateVoiceStateData> {
		using ValueType = DiscordCoreLoader::UpdateVoiceStateData;
		constexpr static auto parseValue =
			object("channel_id", &ValueType::channelId, "guild_id", &ValueType::guildId, "self_deaf", &ValueType::selfDeaf, "self_mute", &ValueType::selfMute);
	};

	template<> struct Core<DiscordCoreLoader::ChannelData> {
		using ValueType					 = DiscordCoreLoader::ChannelData;
		constexpr static auto parseValue = object("guild_id", &ValueType::guildId, "flags", &ValueType::flags, "id", &ValueType::id, "member_count", &ValueType::memberCount,
			"name", &ValueType::name, "owner_id", &ValueType::ownerId, "parent_id", &ValueType::parentId, "permission_overwrites", &ValueType::permissionOverwrites, "topic",
			&ValueType::topic, "type", &ValueType::type);
	};

	template<> struct Core<DiscordCoreLoader::GuildWidgetData> {
		using ValueType					 = DiscordCoreLoader::GuildWidgetData;
		constexpr static auto parseValue = object("channel_id", &ValueType::channelId, "enabled", &ValueType::enabled);
	};

	template<> struct Core<DiscordCoreLoader::GuildMemberData> {
		using ValueType					 = DiscordCoreLoader::GuildMemberData;
		constexpr static auto parseValue = object("flags", &ValueType::flags, "guild_id", &ValueType::guildId, "joined_at", &ValueType::joinedAt, "nick", &ValueType::nick,
			"permissions", &ValueType::permissions, "roles", &ValueType::roles, "user", &ValueType::user);
	};

	template<> struct Core<DiscordCoreLoader::ThreadListSyncData> {
		using ValueType = DiscordCoreLoader::ThreadListSyncData;
		constexpr static auto parseValue =
			object("members", &ValueType::members, "channel_ids", &ValueType::channelIds, "threads", &ValueType::threads, "guild_id", &ValueType::guildId);
	};

	template<> struct Core<DiscordCoreLoader::ResolvedData> {
		using ValueType					 = DiscordCoreLoader::ResolvedData;
		constexpr static auto parseValue = object("attachments", &ValueType::attachments, "members", &ValueType::members, "messages", &ValueType::messages, "channels",
			&ValueType::channels, "users", &ValueType::users, "roles", &ValueType::roles);
	};

	template<> struct Core<DiscordCoreLoader::MessageData> {
		using ValueType					 = DiscordCoreLoader::MessageData;
		constexpr static auto parseValue = object("activity", &ValueType::activity, "application", &ValueType::application, "application_id", &ValueType::applicationId,
			"attachments", &ValueType::attachments, "author", &ValueType::author, "channel_id", &ValueType::channelId, "components", &ValueType::components, "content",
			&ValueType::content, "edited_timestamp", &ValueType::editedTimestamp, "embeds", &ValueType::embeds, "flags", &ValueType::flags, "guild_id", &ValueType::guildId, "id",
			&ValueType::id, "interaction", &ValueType::interaction, "mention_channels", &ValueType::mentionChannels, "mention_everyone", &ValueType::mentionEveryone,
			"mention_roles", &ValueType::mentionRoles, "mentions", &ValueType::mentions, "message_reference", &ValueType::messageReference, "nonce", &ValueType::nonce, "pinned",
			&ValueType::pinned, "reactions", &ValueType::reactions, "sticker_items", &ValueType::stickerItems, "stickers", &ValueType::stickers, "thread", &ValueType::thread,
			"tts", &ValueType::tts, "type", &ValueType::type);
	};

	template<> struct Core<DiscordCoreLoader::ApplicationCommandInteractionDataOption> {
		using ValueType					 = DiscordCoreLoader::ApplicationCommandInteractionDataOption;
		constexpr static auto parseValue = object("name", &ValueType::name, "type", &ValueType::type, "focused", &ValueType::focused, "options", &ValueType::options);
	};

	template<> struct Core<DiscordCoreLoader::InteractionData> {
		using ValueType					 = DiscordCoreLoader::InteractionData;
		constexpr static auto parseValue = object("data", &ValueType::data, "guild_locale", &ValueType::guildLocale, "application_id", &ValueType::applicationId, "member",
			&ValueType::member, "type", &ValueType::type, "message", &ValueType::message, "channel_id", &ValueType::channelId, "locale", &ValueType::locale, "guild_id",
			&ValueType::guildId, "token", &ValueType::token, "version", &ValueType::version, "user", &ValueType::user, "id", &ValueType::id);
	};

	template<> struct Core<DiscordCoreLoader::ThreadMembersUpdateData> {
		using ValueType					 = DiscordCoreLoader::ThreadMembersUpdateData;
		constexpr static auto parseValue = object("added_members", &ValueType::addedMembers, "member_count", &ValueType::memberCount, "guild_id", &ValueType::guildId,
			"removed_member_ids", &ValueType::removedMemberIds);
	};

	template<> struct Core<DiscordCoreLoader::WebHookData> {
		using ValueType					 = DiscordCoreLoader::WebHookData;
		constexpr static auto parseValue = object("application_id", &ValueType::applicationId, "avatar", &ValueType::avatar, "channel_id", &ValueType::channelId, "guild_id",
			&ValueType::guildId, "id", &ValueType::id, "name", &ValueType::name, "source_channel", &ValueType::sourceChannel, "source_guild", &ValueType::sourceGuild, "token",
			&ValueType::token, "type", &ValueType::type, "url", &ValueType::url, "user", &ValueType::user);
	};

	template<> struct Core<DiscordCoreLoader::AuditLogChangeData> {
		using ValueType					 = DiscordCoreLoader::AuditLogChangeData;
		constexpr static auto parseValue = object("key", &ValueType::key, "new_value", &ValueType::newValue, "old_value", &ValueType::oldValue);
	};

	template<> struct Core<DiscordCoreLoader::OptionalAuditEntryInfoData> {
		using ValueType = DiscordCoreLoader::OptionalAuditEntryInfoData;
		constexpr static auto parseValue =
			object("delete_member_days", &ValueType::deleteMemberDays, "members_removed", &ValueType::membersRemoved, "application_id", &ValueType::applicationId, "role_name",
				&ValueType::roleName, "channel_id", &ValueType::channelId, "message_id", &ValueType::messageId, "count", &ValueType::count, "type", &ValueType::type);
	};

	template<> struct Core<DiscordCoreLoader::ArchivedThreadsData> {
		using ValueType					 = DiscordCoreLoader::ArchivedThreadsData;
		constexpr static auto parseValue = object("has_more", &ValueType::hasMore, "member", &ValueType::members, "threads", &ValueType::threads);
	};

	template<> struct Core<DiscordCoreLoader::ConnectionData> {
		using ValueType = DiscordCoreLoader::ConnectionData;
		constexpr static auto parseValue =
			object("friend_sync", &ValueType::friendSync, "id", &ValueType::id, "integrations", &ValueType::integrations, "name", &ValueType::name, "revoked", &ValueType::revoked,
				"show_activity", &ValueType::showActivity, "type", &ValueType::type, "verified", &ValueType::verified, "visibility", &ValueType::visibility);
	};

	template<> struct Core<DiscordCoreLoader::GuildPruneCountData> {
		using ValueType					 = DiscordCoreLoader::GuildPruneCountData;
		constexpr static auto parseValue = object("count", &ValueType::count);
	};

	template<> struct Core<DiscordCoreLoader::VoiceRegionData> {
		using ValueType = DiscordCoreLoader::VoiceRegionData;
		constexpr static auto parseValue =
			object("custom", &ValueType::custom, "depracated", &ValueType::deprecated, "id", &ValueType::id, "name", &ValueType::name, "optimal", &ValueType::optimal);
	};

	template<> struct Core<DiscordCoreLoader::InviteData> {
		using ValueType					 = DiscordCoreLoader::InviteData;
		constexpr static auto parseValue = object("approximate_member_count", &ValueType::approximateMemberCount, "approximate_presence_count",
			&ValueType::approximatePresenceCount, "channel", &ValueType::channel, "guild_id", &ValueType::guildId, "code", &ValueType::code, "created_at", &ValueType::createdAt,
			"expires_at", &ValueType::expiresAt, "guild", &ValueType::guild, "guild_scheduled_event", &ValueType::guildScheduledEvent, "inviter", &ValueType::inviter, "max_age",
			&ValueType::maxAge, "max_uses", &ValueType::maxUses, "stage_instance", &ValueType::stageInstance, "target_application", &ValueType::targetApplication, "target_type",
			&ValueType::targetType, "target_user", &ValueType::targetUser, "temporary", &ValueType::temporary, "uses", &ValueType::uses);
	};

	template<> struct Core<DiscordCoreLoader::ApplicationCommandData> {
		using ValueType					 = DiscordCoreLoader::ApplicationCommandData;
		constexpr static auto parseValue = object("application_id", &ValueType::applicationId, "default_member_permissions", &ValueType::defaultMemberPermissions, "description",
			&ValueType::description, "guild_id", &ValueType::guildId, "description_localizations", &ValueType::descriptionLocalizations, "dm_permission", &ValueType::dmPermission,
			"id", &ValueType::id, "name", &ValueType::name, "name_localizations", &ValueType::nameLocalizations, "options", &ValueType::options, "type", &ValueType::type,
			"version", &ValueType::version);
	};

	template<> struct Core<DiscordCoreLoader::StickerData> {
		using ValueType					 = DiscordCoreLoader::StickerData;
		constexpr static auto parseValue = object("asset", &ValueType::asset, "description", &ValueType::description, "format_type", &ValueType::formatType, "id", &ValueType::id,
			"name", &ValueType::name, "pack_id", &ValueType::packId, "tags", &ValueType::tags, "type", &ValueType::type, "user", &ValueType::user, "guild_id", &ValueType::guildId,
			"sort_value", &ValueType::sortValue, "nsfw_level", &ValueType::nsfwLevel);
	};

	template<> struct Core<DiscordCoreLoader::MessageActivityData> {
		using ValueType					 = DiscordCoreLoader::MessageActivityData;
		constexpr static auto parseValue = object("type", &ValueType::type, "party_id", &ValueType::partyId);
	};

	template<> struct Core<DiscordCoreLoader::ActiveThreadsData> {
		using ValueType					 = DiscordCoreLoader::ActiveThreadsData;
		constexpr static auto parseValue = object("has_more", &ValueType::hasMore, "members", &ValueType::members, "threads", &ValueType::threads);
	};

	template<> struct Core<DiscordCoreLoader::ActivityData> {
		using ValueType					 = DiscordCoreLoader::ActivityData;
		constexpr static auto parseValue = object("name", &ValueType::name, "type", &ValueType::type, "url", &ValueType::url);
	};

	template<> struct Core<DiscordCoreLoader::PresenceUpdateData> {
		using ValueType					 = DiscordCoreLoader::PresenceUpdateData;
		constexpr static auto parseValue = object("client_status", &ValueType::clientStatus, "guild_id", &ValueType::guildId, "status", &ValueType::status, "user",
			&ValueType::user, "activities", &ValueType::activities);
	};

	template<> struct Core<DiscordCoreLoader::AccountData> {
		using ValueType					 = DiscordCoreLoader::AccountData;
		constexpr static auto parseValue = object("name", &ValueType::name);
	};

	template<> struct Core<DiscordCoreLoader::EmbedData> {
		using ValueType					 = DiscordCoreLoader::EmbedData;
		constexpr static auto parseValue = object("author", &ValueType::author, "color", &ValueType::hexColorValue, "description", &ValueType::description, "fields",
			&ValueType::fields, "footer", &ValueType::footer, "image", &ValueType::image, "provider", &ValueType::provider, "thumbnail", &ValueType::thumbnail, "title",
			&ValueType::title, "type", &ValueType::type, "url", &ValueType::url, "video", &ValueType::video);
	};

	template<> struct Core<DiscordCoreLoader::MessageDataOld> {
		using ValueType					 = DiscordCoreLoader::MessageDataOld;
		constexpr static auto parseValue = object("activity", &ValueType::activity, "application", &ValueType::application, "application_id", &ValueType::applicationId,
			"attachments", &ValueType::attachments, "author", &ValueType::author, "channel_id", &ValueType::channelId, "components", &ValueType::components, "content",
			&ValueType::content, "edited_timestamp", &ValueType::editedTimestamp, "embeds", &ValueType::embeds, "flags", &ValueType::flags, "guild_id", &ValueType::guildId, "id",
			&ValueType::id, "interaction", &ValueType::interaction, "mention_channels", &ValueType::mentionChannels, "mention_everyone", &ValueType::mentionEveryone,
			"mention_roles", &ValueType::mentionRoles, "mentions", &ValueType::mentions, "message_reference", &ValueType::messageReference, "nonce", &ValueType::nonce, "pinned",
			&ValueType::pinned, "reactions", &ValueType::reactions, "sticker_items", &ValueType::stickerItems, "stickers", &ValueType::stickers, "thread", &ValueType::thread,
			"tts", &ValueType::tts, "type", &ValueType::type);
	};

	template<> struct Core<DiscordCoreLoader::ReactionData> {
		using ValueType					 = DiscordCoreLoader::ReactionData;
		constexpr static auto parseValue = object("channel_id", &ValueType::channelId, "count", &ValueType::count, "emoji", &ValueType::emoji, "guild_id", &ValueType::guildId,
			"id", &ValueType::id, "member", &ValueType::member, "me", &ValueType::me, "message_id", &ValueType::messageId, "user_id", &ValueType::userId);
	};

	template<> struct Core<DiscordCoreLoader::StickerItemData> {
		using ValueType					 = DiscordCoreLoader::StickerItemData;
		constexpr static auto parseValue = object("format_type", &ValueType::formatType, "name", &ValueType::name);
	};

	template<> struct Core<DiscordCoreLoader::AttachmentData> {
		using ValueType					 = DiscordCoreLoader::AttachmentData;
		constexpr static auto parseValue = object("content_type", &ValueType::contentType, "description", &ValueType::description, "filename", &ValueType::filename, "height",
			&ValueType::height, "id", &ValueType::id, "proxy_url", &ValueType::proxyUrl, "size", &ValueType::size, "url", &ValueType::url, "width", &ValueType::width, "ephemeral",
			&ValueType::ephemeral);
	};

	template<> struct Core<DiscordCoreLoader::ChannelMentionData> {
		using ValueType					 = DiscordCoreLoader::ChannelMentionData;
		constexpr static auto parseValue = object("id", &ValueType::id, "guild_id", &ValueType::guildId, "name", &ValueType::name, "type", &ValueType::type);
	};

	template<> struct Core<DiscordCoreLoader::ActionRowData> {
		using ValueType					 = DiscordCoreLoader::ActionRowData;
		constexpr static auto parseValue = object("components", &ValueType::components);
	};

	template<> struct Core<DiscordCoreLoader::EmbedFooterData> {
		using ValueType					 = DiscordCoreLoader::EmbedFooterData;
		constexpr static auto parseValue = object("proxy_icon_url", &ValueType::proxyIconUrl, "icon_url", &ValueType::iconUrl, "text", &ValueType::text);
	};

	template<> struct Core<DiscordCoreLoader::EmbedImageData> {
		using ValueType					 = DiscordCoreLoader::EmbedImageData;
		constexpr static auto parseValue = object("proxy_url", &ValueType::proxyUrl, "url", &ValueType::url, "height", &ValueType::height, "width", &ValueType::width);
	};

	template<> struct Core<DiscordCoreLoader::EmbedThumbnailData> {
		using ValueType					 = DiscordCoreLoader::EmbedThumbnailData;
		constexpr static auto parseValue = object("proxy_url", &ValueType::proxyUrl, "url", &ValueType::url, "height", &ValueType::height, "width", &ValueType::width);
	};

	template<> struct Core<DiscordCoreLoader::EmbedVideoData> {
		using ValueType					 = DiscordCoreLoader::EmbedVideoData;
		constexpr static auto parseValue = object("proxy_url", &ValueType::proxyUrl, "url", &ValueType::url, "height", &ValueType::height, "width", &ValueType::width);
	};

	template<> struct Core<DiscordCoreLoader::EmbedProviderData> {
		using ValueType					 = DiscordCoreLoader::EmbedProviderData;
		constexpr static auto parseValue = object("name", &ValueType::name, "url", &ValueType::url);
	};

	template<> struct Core<DiscordCoreLoader::EmbedAuthorData> {
		using ValueType					 = DiscordCoreLoader::EmbedAuthorData;
		constexpr static auto parseValue = object("proxy_icon_url", &ValueType::proxyIconUrl, "icon_url", &ValueType::iconUrl, "name", &ValueType::name, "url", &ValueType::url);
	};

	template<> struct Core<DiscordCoreLoader::EmbedFieldData> {
		using ValueType					 = DiscordCoreLoader::EmbedFieldData;
		constexpr static auto parseValue = object("name", &ValueType::name, "value", &ValueType::value, "inline", &ValueType::Inline);
	};

	template<> struct Core<DiscordCoreLoader::AuditLogEntryData> {
		using ValueType					 = DiscordCoreLoader::AuditLogEntryData;
		constexpr static auto parseValue = object("action_type", &ValueType::actionType, "changes", &ValueType::changes, "created_at", &ValueType::createdTimeStamp, "id",
			&ValueType::id, "options", &ValueType::options, "reason", &ValueType::reason, "target_id", &ValueType::targetId, "user_id", &ValueType::userId);
	};

	template<> struct Core<DiscordCoreLoader::ComponentData> {
		using ValueType					 = DiscordCoreLoader::ComponentData;
		constexpr static auto parseValue = object("type", &ValueType::type, "custom_id", &ValueType::customId, "options", &ValueType::options, "placeholder",
			&ValueType::placeholder, "min_values", &ValueType::minValues, "max_values", &ValueType::maxValues, "min_length", &ValueType::minLength, "max_length",
			&ValueType::maxLength, "label", &ValueType::label, "value", &ValueType::value, "title", &ValueType::title, "emoji", &ValueType::emoji, "url", &ValueType::url,
			"required", &ValueType::required, "disabled", &ValueType::disabled, "style", &ValueType::style);
	};

	template<> struct Core<DiscordCoreLoader::SelectOptionData> {
		using ValueType = DiscordCoreLoader::SelectOptionData;
		constexpr static auto parseValue =
			object("description", &ValueType::description, "label", &ValueType::label, "value", &ValueType::value, "emoji", &ValueType::emoji, "default", &ValueType::_default);
	};

	template<> struct Core<DiscordCoreLoader::ClientStatusData> {
		using ValueType					 = DiscordCoreLoader::ClientStatusData;
		constexpr static auto parseValue = object("desktop", &ValueType::desktop, "mobile", &ValueType::mobile, "web", &ValueType::web);
	};

	template<> struct Core<DiscordCoreLoader::GuildData> {
		using ValueType					 = DiscordCoreLoader::GuildData;
		constexpr static auto parseValue = object("id", &ValueType::id, "roles", &ValueType::roles, "channels", &ValueType::channels, "members", &ValueType::members);
	};

	template<> struct Core<DiscordCoreLoader::IntegrationData> {
		using ValueType					 = DiscordCoreLoader::IntegrationData;
		constexpr static auto parseValue = object("account", &ValueType::account, "application", &ValueType::application, "enabled", &ValueType::enabled, "enable_emoticons",
			&ValueType::enableEmoticons, "expire_behavior", &ValueType::expireBehavior, "expire_grace_period", &ValueType::expireGracePeriod, "id", &ValueType::id, "name",
			&ValueType::name, "revoked", &ValueType::revoked, "role_id", &ValueType::roleId, "subscriber_count", &ValueType::subscriberCount, "syncing", &ValueType::syncing,
			"synced_at", &ValueType::syncedAt, "type", &ValueType::type, "user", &ValueType::user);
	};

	template<> struct Core<DiscordCoreLoader::File> {
		using ValueType					 = DiscordCoreLoader::File;
		static constexpr auto parseValue = array(&ValueType::data);
	};

	template<> struct Core<DiscordCoreLoader::AllowedMentionsData> {
		using ValueType					 = DiscordCoreLoader::AllowedMentionsData;
		static constexpr auto parseValue = object("replied_user", &ValueType::repliedUser, "parse", &ValueType::parse, "roles", &ValueType::roles, "users", &ValueType::users);
	};

	template<> struct Core<DiscordCoreLoader::MessageReferenceData> {
		using ValueType = DiscordCoreLoader::MessageReferenceData;
		constexpr static auto parseValue =
			object("fail_if_not_exists", &ValueType::failIfNotExists, "message_id", &ValueType::messageId, "channel_id", &ValueType::channelId, "guild_id", &ValueType::guildId);
	};

	template<> struct Core<DiscordCoreLoader::MessageInteractionData> {
		using ValueType					 = DiscordCoreLoader::MessageInteractionData;
		constexpr static auto parseValue = object("id", &ValueType::id, "member", &ValueType::member, "name", &ValueType::name, "type", &ValueType::type, "user", &ValueType::user);
	};

	template<> struct Core<DiscordCoreLoader::GuildScheduledEventData> {
		using ValueType					 = DiscordCoreLoader::GuildScheduledEventData;
		constexpr static auto parseValue = object("id", &ValueType::id, "privacy_level", &ValueType::privacyLevel, "entity_metadata", &ValueType::entityMetadata, "entity_type",
			&ValueType::entityType, "status", &ValueType::status, "scheduled_start_time", &ValueType::scheduledStartTime, "scheduled_end_time", &ValueType::scheduledEndTime,
			"description", &ValueType::description, "creator_id", &ValueType::creatorId, "channel_id", &ValueType::channelId, "entity_id", &ValueType::entityId, "user_count",
			&ValueType::userCount, "guild_id", &ValueType::guildId, "creator", &ValueType::creator, "name", &ValueType::name);
	};

	template<> struct Core<DiscordCoreLoader::GuildScheduledEventUserData> {
		using ValueType					 = DiscordCoreLoader::GuildScheduledEventUserData;
		constexpr static auto parseValue = object("guild_scheduled_event_id", &ValueType::guildScheduledEventId, "member", &ValueType::member, "user", &ValueType::user);
	};

	template<> struct Core<DiscordCoreLoader::GuildScheduledEventMetadata> {
		using ValueType					 = DiscordCoreLoader::GuildScheduledEventMetadata;
		constexpr static auto parseValue = object("location", &ValueType::location);
	};

	template<> struct Core<DiscordCoreLoader::RoleData> {
		using ValueType					 = DiscordCoreLoader::RoleData;
		constexpr static auto parseValue = object("color", &ValueType::color, "flags", &ValueType::flags, "id", &ValueType::id, "name", &ValueType::name, "permissions",
			&ValueType::permissions, "position", &ValueType::position, "unicode_emoji", &ValueType::unicodeEmoji);
	};

	template<> struct Core<DiscordCoreLoader::StickerPackData> {
		using ValueType					 = DiscordCoreLoader::StickerPackData;
		constexpr static auto parseValue = object("banner_asset_id", &ValueType::bannerAssetId, "cover_sticker_id", &ValueType::coverStickerId, "description",
			&ValueType::description, "name", &ValueType::name, "sku_id", &ValueType::skuId, "stickers", &ValueType::stickers);
	};

	template<> struct Core<DiscordCoreLoader::GuildWidgetImageData> {
		using ValueType					 = DiscordCoreLoader::GuildWidgetImageData;
		constexpr static auto parseValue = object("url", &ValueType::url);
	};

	template<> struct Core<DiscordCoreLoader::GuildTemplateData> {
		using ValueType					 = DiscordCoreLoader::GuildTemplateData;
		static constexpr auto parseValue = object("code", &ValueType::code, "created_at", &ValueType::createdAt, "creator", &ValueType::creator, "creator_id",
			&ValueType::creatorId, "description", &ValueType::description, "is_dirty", &ValueType::isDirty, "name", &ValueType::name, "serialized_source_guild",
			&ValueType::serializedSourceGuild, "source_guild_id", &ValueType::sourceGuildId, "updated_at", &ValueType::updatedAt);
	};

	template<> struct Core<DiscordCoreLoader::ApplicationCommandOptionData> {
		using ValueType					 = DiscordCoreLoader::ApplicationCommandOptionData;
		constexpr static auto parseValue = object("type", &ValueType::type, "name", &ValueType::name, "description", &ValueType::description, "required", &ValueType::required,
			"autocomplete", &ValueType::autocomplete, "min_value", &ValueType::minValue, "max_value", &ValueType::maxValue, "channel_types", &ValueType::channelTypes,
			"description_localizations", &ValueType::descriptionLocalizations, "name_localizations", &ValueType::nameLocalizations, "choices", &ValueType::choices, "options",
			&ValueType::options);
	};

	template<> struct Core<DiscordCoreLoader::StageInstanceData> {
		using ValueType					 = DiscordCoreLoader::StageInstanceData;
		constexpr static auto parseValue = object("channel_id", &ValueType::channelId, "discoverable_disabled", &ValueType::discoverableDisabled, "guild_id", &ValueType::guildId,
			"id", &ValueType::id, "privacy_level", &ValueType::privacyLevel, "topic", &ValueType::topic);
	};

	template<> struct Core<DiscordCoreLoader::ApplicationCommandOptionChoiceData> {
		using ValueType					 = DiscordCoreLoader::ApplicationCommandOptionChoiceData;
		constexpr static auto parseValue = object("name", &ValueType::name, "value", &ValueType::value, "name_localized", &ValueType::nameLocalizations);
	};

	template<> struct Core<DiscordCoreLoader::RoleTagsData> {
		using ValueType					 = DiscordCoreLoader::RoleTagsData;
		constexpr static auto parseValue = object("premium_subscriber", &ValueType::premiumSubscriber, "integration_id", &ValueType::integrationId, "bot_id", &ValueType::botId);
	};

	template<typename ValueTypeNew> struct Core<DiscordCoreLoader::WebSocketMessageReal<ValueTypeNew>> {
		using ValueType					 = DiscordCoreLoader::WebSocketMessageReal<ValueTypeNew>;
		constexpr static auto parseValue = object("d", &ValueType::d, "t", &ValueType::t, "s", &ValueType::s, "op", &ValueType::op);
	};

	template<> struct Core<DiscordCoreLoader::WebSocketMessageReal<DiscordCoreLoader::GuildData>> {
		using ValueType					 = DiscordCoreLoader::WebSocketMessageReal<DiscordCoreLoader::GuildData>;
		constexpr static auto parseValue = object("d", &ValueType::d, "t", &ValueType::t, "s", &ValueType::s, "op", &ValueType::op);
	};

	template<> struct Core<DiscordCoreLoader::WebSocketMessage> {
		using ValueType					 = DiscordCoreLoader::WebSocketMessage;
		constexpr static auto parseValue = object("d", &ValueType::d, "t", &ValueType::t, "s", &ValueType::s, "op", &ValueType::op);
	};

	template<> struct Core<DiscordCoreLoader::ReadyData> {
		using ValueType = DiscordCoreLoader::ReadyData;
		constexpr static auto parseValue =
			object("resume_gateway_url", &ValueType::resumeGateWayUrl, "session_id", &ValueType::sessionId, "user", &ValueType::user, "v", &ValueType::v);
	};

	template<> struct Core<DiscordCoreLoader::HelloData> {
		using ValueType					 = DiscordCoreLoader::HelloData;
		constexpr static auto parseValue = object("heartbeat_interval", &ValueType::heartBeatInterval);
	};

	template<> struct Core<DiscordCoreLoader::WebSocketIdentifyData> {
		using ValueType					 = DiscordCoreLoader::WebSocketIdentifyData;
		constexpr static auto parseValue = object("intents", &ValueType::intents, "shard", &ValueType::shard);
	};

	template<> struct Core<DiscordCoreLoader::ConfigData> {
		using ValueType					 = DiscordCoreLoader::ConfigData;
		constexpr static auto parseValue = object("ConnectionIp", &ValueType::connectionIp, "ConnectionPort", &ValueType::connectionPort,
			"DoWePrintWebSocketSuccessReceiveMessages", &ValueType::doWePrintWebSocketSuccessReceiveMessages, "DoWePrintWebSocketSuccessSentMessages",
			&ValueType::doWePrintWebSocketSuccessSentMessages, "DoWePrintWebSocketErrorMessages", &ValueType::doWePrintWebSocketErrorMessages, "DoWePrintGeneralSuccessMessages",
			&ValueType::doWePrintGeneralSuccessMessages, "DoWePrintGeneralErrorMessages", &ValueType::doWePrintGeneralErrorMessages, "GuildQuantity", &ValueType::guildQuantity,
			"StdDeviationForStringLength", &ValueType::stdDeviationForStringLength, "MeanForStringLength", &ValueType::meanForStringLength, "StdDeviationForMemberCount",
			&ValueType::stdDeviationForMemberCount, "MeanForMemberCount", &ValueType::meanForMemberCount, "StdDeviationForChannelCount", &ValueType::stdDeviationForChannelCount,
			"MeanForChannelCount", &ValueType::meanForChannelCount, "StdDeviationForRoleCount", &ValueType::stdDeviationForRoleCount, "MeanForRoleCount",
			&ValueType::meanForRoleCount, "TextFormat", &ValueType::format);
	};


};// namespace DiscordCoreLoader

/**@}*/
