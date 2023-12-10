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
/// FoundationEntities.hpp - Header for all of the Discord/Support API data
/// May 22, 2022
/// https://github.com/RealTimeChris/discord_core_loader
/// \file FoundationEntities.hpp

#pragma once

#include <discordcoreloader/FoundationEntities.hpp>
#include <discordcoreloader/SSLClients.hpp>
#include <jsonifier/Index.hpp>

namespace jsonifier_internal {

	template<typename value_type>
	concept snowflake_t = std::same_as<discord_core_loader::snowflake, jsonifier::concepts::unwrap<value_type>>;

	template<snowflake_t value_type_new> struct serialize_impl<value_type_new> {
		template<snowflake_t value_type, jsonifier::concepts::buffer_like iterator_type>
		inline static void op(value_type&& value, iterator_type&& iter, uint64_t& index) {
			jsonifier::string newString{ static_cast<jsonifier::string>(value) };
			serialize::op(newString, iter, index);
		}
	};
}

namespace jsonifier {


	template<> struct core<discord_core_loader::ApplicationCommandPermissionData> {
		using ValueType					 = discord_core_loader::ApplicationCommandPermissionData;
		constexpr static auto parseValue = createValue("id", &ValueType::id, "permission", &ValueType::permission, "type", &ValueType::type);
	};

	template<> struct core<discord_core_loader::GuildApplicationCommandPermissionsData> {
		using ValueType = discord_core_loader::GuildApplicationCommandPermissionsData;
		constexpr static auto parseValue =
			createValue("application_id", &ValueType::applicationId, "permissions", &ValueType::permissions, "guild_id", &ValueType::guildId, "id", &ValueType::id);
	};

	template<> struct core<discord_core_loader::SessionStartData> {
		using ValueType = discord_core_loader::SessionStartData;
		constexpr static auto parseValue =
			createValue("max_concurrency", &ValueType::maxConcurrency, "remaining", &ValueType::remaining, "reset_after", &ValueType::resetAfter, "total", &ValueType::total);
	};

	template<> struct core<discord_core_loader::AuthorizationInfoData> {
		using ValueType					 = discord_core_loader::AuthorizationInfoData;
		constexpr static auto parseValue = createValue("application", &ValueType::application, "expires", &ValueType::expires, "scopes", &ValueType::scopes, "user", &ValueType::user);
	};

	template<> struct core<discord_core_loader::EmojiData> {
		using ValueType					 = discord_core_loader::EmojiData;
		constexpr static auto parseValue = createValue("id", &ValueType::id, "name", &ValueType::name, "roles", &ValueType::roles, "require_colons", &ValueType::requireColons,
			"managed", &ValueType::managed, "animated", &ValueType::animated, "available", &ValueType::available);
	};

	template<> struct core<discord_core_loader::TeamMembersObjectData> {
		using ValueType					 = discord_core_loader::TeamMembersObjectData;
		constexpr static auto parseValue = createValue("membership_state", &ValueType::membershipState, "team_id", &ValueType::teamId, "user", &ValueType::user);
	};

	template<> struct core<discord_core_loader::TeamObjectData> {
		using ValueType					 = discord_core_loader::TeamObjectData;
		constexpr static auto parseValue = createValue("icon", &ValueType::icon, "id", &ValueType::id, "members", &ValueType::members, "owner_user_id", &ValueType::ownerUserId);
	};

	template<> struct core<discord_core_loader::InstallParamsData> {
		using ValueType					 = discord_core_loader::InstallParamsData;
		constexpr static auto parseValue = createValue("scopes", &ValueType::scopes, "permissions", &ValueType::permissions);
	};

	template<> struct core<discord_core_loader::ApplicationData> {
		using ValueType					 = discord_core_loader::ApplicationData;
		constexpr static auto parseValue = createValue("bot_public", &ValueType::botPublic, "bot_require_code_grant", &ValueType::botRequireCodeGrant, "cover_image",
			&ValueType::coverImage, "description", &ValueType::description, "", "flags", &ValueType::flags, "guild_id", &ValueType::guildId, "icon", &ValueType::icon, "name",
			&ValueType::name, "owner", &ValueType::owner, "params", &ValueType::params, "primary_sku_id", &ValueType::primarySkuId, "privacy_policy_url",
			&ValueType::privacyPolicyUrl, "rpc_origins", &ValueType::rpcOrigins, "slug", &ValueType::slug, "summary", &ValueType::summary, "tags", &ValueType::tags, "team",
			&ValueType::team, "terms_of_service_url", &ValueType::termsOfServiceUrl, "verify_key", &ValueType::verifyKey);
	};

	template<> struct core<discord_core_loader::ThreadMetadataData> {
		using ValueType					 = discord_core_loader::ThreadMetadataData;
		constexpr static auto parseValue = createValue("archived", &ValueType::archived, "archived_timestamp", &ValueType::archiveTimestamp, "auto_archive_duration",
			&ValueType::autoArchiveDuration, "invitable", &ValueType::invitable, "locked", &ValueType::locked);
	};

	template<> struct core<discord_core_loader::GatewayBotData> {
		using ValueType					 = discord_core_loader::GatewayBotData;
		constexpr static auto parseValue = createValue("session_start_limit", &ValueType::sessionStartLimit, "shards", &ValueType::shards, "url", &ValueType::url);
	};

	template<> struct core<discord_core_loader::DiscordEntity> {
		using ValueType					 = discord_core_loader::DiscordEntity;
		constexpr static auto parseValue = createValue("id", &ValueType::id);
	};

	template<> struct core<discord_core_loader::UserData> {
		using ValueType					 = discord_core_loader::UserData;
		constexpr static auto parseValue = createValue("mfa_enabled", &ValueType::flags, "verified", &ValueType::flags, "system", &ValueType::flags, "bot", &ValueType::flags,
			"public_flags", &ValueType::flags, "username", &ValueType::userName, "discriminator", &ValueType::discriminator, "id", &ValueType::id, "avatar", &ValueType::avatar);
	};

	template<> struct core<discord_core_loader::WelcomeScreenChannelData> {
		using ValueType = discord_core_loader::WelcomeScreenChannelData;
		constexpr static auto parseValue =
			createValue("channel_id", &ValueType::channelId, "description", &ValueType::description, "emoji_id", &ValueType::emojiId, "emoji_name", &ValueType::emojiName);
	};

	template<> struct core<discord_core_loader::WelcomeScreenData> {
		using ValueType					 = discord_core_loader::WelcomeScreenData;
		constexpr static auto parseValue = createValue("description", &ValueType::description, "welcome_screen_channels", &ValueType::welcomeChannels);
	};

	template<> struct core<discord_core_loader::GuildPreviewData> {
		using ValueType					 = discord_core_loader::GuildPreviewData;
		constexpr static auto parseValue = createValue("approximate_member_count", &ValueType::approximateMemberCount, "approximate_presence_count",
			&ValueType::approximatePresenceCount, "description", &ValueType::description, "discovery_splash", &ValueType::discoverySplash, "emojis", &ValueType::emojis, "features",
			&ValueType::features, "icon", &ValueType::icon, "id", &ValueType::id, "name", &ValueType::name, "splash", &ValueType::splash, "stickers", &ValueType::stickers);
	};

	template<> struct core<discord_core_loader::ThreadMemberData> {
		using ValueType					 = discord_core_loader::ThreadMemberData;
		constexpr static auto parseValue = createValue("flags", &ValueType::flags, "join_timestamp", &ValueType::joinTimestamp, "user_id", &ValueType::userId);
	};

	template<> struct core<discord_core_loader::AuditLogData> {
		using ValueType					 = discord_core_loader::AuditLogData;
		constexpr static auto parseValue = createValue("audit_log_entries", &ValueType::auditLogEntries, "guild_scheduled_events", &ValueType::guildScheduledEvents, "integrations",
			&ValueType::integrations, "threads", &ValueType::threads, "users", &ValueType::users, "webhooks", &ValueType::webhooks);
	};

	template<> struct core<discord_core_loader::BanData> {
		using ValueType					 = discord_core_loader::BanData;
		constexpr static auto parseValue = createValue("failed_due_to_perms", &ValueType::failedDueToPerms, "reason", &ValueType::reason, "user", &ValueType::user);
	};

	template<> struct core<discord_core_loader::OverWriteData> {
		using ValueType					 = discord_core_loader::OverWriteData;
		constexpr static auto parseValue = createValue("channel_id", &ValueType::channelId, "id", &ValueType::id, "type", &ValueType::type);
	};

	template<> struct core<discord_core_loader::UpdateVoiceStateData> {
		using ValueType = discord_core_loader::UpdateVoiceStateData;
		constexpr static auto parseValue =
			createValue("channel_id", &ValueType::channelId, "guild_id", &ValueType::guildId, "self_deaf", &ValueType::selfDeaf, "self_mute", &ValueType::selfMute);
	};

	template<> struct core<discord_core_loader::ChannelData> {
		using ValueType					 = discord_core_loader::ChannelData;
		constexpr static auto parseValue = createValue("guild_id", &ValueType::guildId, "flags", &ValueType::flags, "id", &ValueType::id, "member_count", &ValueType::memberCount,
			"name", &ValueType::name, "owner_id", &ValueType::ownerId, "parent_id", &ValueType::parentId, "permission_overwrites", &ValueType::permissionOverwrites, "topic",
			&ValueType::topic, "type", &ValueType::type);
	};

	template<> struct core<discord_core_loader::GuildWidgetData> {
		using ValueType					 = discord_core_loader::GuildWidgetData;
		constexpr static auto parseValue = createValue("channel_id", &ValueType::channelId, "enabled", &ValueType::enabled);
	};

	template<> struct core<discord_core_loader::GuildMemberData> {
		using ValueType					 = discord_core_loader::GuildMemberData;
		constexpr static auto parseValue = createValue("flags", &ValueType::flags, "guild_id", &ValueType::guildId, "joined_at", &ValueType::joinedAt, "nick", &ValueType::nick,
			"permissions", &ValueType::permissions, "roles", &ValueType::roles, "user", &ValueType::user);
	};

	template<> struct core<discord_core_loader::ThreadListSyncData> {
		using ValueType = discord_core_loader::ThreadListSyncData;
		constexpr static auto parseValue =
			createValue("members", &ValueType::members, "channel_ids", &ValueType::channelIds, "threads", &ValueType::threads, "guild_id", &ValueType::guildId);
	};

	template<> struct core<discord_core_loader::ResolvedData> {
		using ValueType					 = discord_core_loader::ResolvedData;
		constexpr static auto parseValue = createValue("attachments", &ValueType::attachments, "members", &ValueType::members, "messages", &ValueType::messages, "channels",
			&ValueType::channels, "users", &ValueType::users, "roles", &ValueType::roles);
	};

	template<> struct core<discord_core_loader::MessageData> {
		using ValueType					 = discord_core_loader::MessageData;
		constexpr static auto parseValue = createValue("activity", &ValueType::activity, "application", &ValueType::application, "application_id", &ValueType::applicationId,
			"attachments", &ValueType::attachments, "author", &ValueType::author, "channel_id", &ValueType::channelId, "components", &ValueType::components, "content",
			&ValueType::content, "edited_timestamp", &ValueType::editedTimestamp, "embeds", &ValueType::embeds, "flags", &ValueType::flags, "guild_id", &ValueType::guildId, "id",
			&ValueType::id, "interaction", &ValueType::interaction, "mention_channels", &ValueType::mentionChannels, "mention_everyone", &ValueType::mentionEveryone,
			"mention_roles", &ValueType::mentionRoles, "mentions", &ValueType::mentions, "message_reference", &ValueType::messageReference, "nonce", &ValueType::nonce, "pinned",
			&ValueType::pinned, "reactions", &ValueType::reactions, "sticker_items", &ValueType::stickerItems, "stickers", &ValueType::stickers, "thread", &ValueType::thread,
			"tts", &ValueType::tts, "type", &ValueType::type);
	};

	template<> struct core<discord_core_loader::ApplicationCommandInteractionDataOption> {
		using ValueType					 = discord_core_loader::ApplicationCommandInteractionDataOption;
		constexpr static auto parseValue = createValue("name", &ValueType::name, "type", &ValueType::type, "focused", &ValueType::focused, "options", &ValueType::options);
	};

	template<> struct core<discord_core_loader::InteractionData> {
		using ValueType					 = discord_core_loader::InteractionData;
		constexpr static auto parseValue = createValue("data", &ValueType::data, "guild_locale", &ValueType::guildLocale, "application_id", &ValueType::applicationId, "member",
			&ValueType::member, "type", &ValueType::type, "message", &ValueType::message, "channel_id", &ValueType::channelId, "locale", &ValueType::locale, "guild_id",
			&ValueType::guildId, "token", &ValueType::token, "version", &ValueType::version, "user", &ValueType::user, "id", &ValueType::id);
	};

	template<> struct core<discord_core_loader::ThreadMembersUpdateData> {
		using ValueType					 = discord_core_loader::ThreadMembersUpdateData;
		constexpr static auto parseValue = createValue("added_members", &ValueType::addedMembers, "member_count", &ValueType::memberCount, "guild_id", &ValueType::guildId,
			"removed_member_ids", &ValueType::removedMemberIds);
	};

	template<> struct core<discord_core_loader::WebHookData> {
		using ValueType					 = discord_core_loader::WebHookData;
		constexpr static auto parseValue = createValue("application_id", &ValueType::applicationId, "avatar", &ValueType::avatar, "channel_id", &ValueType::channelId, "guild_id",
			&ValueType::guildId, "id", &ValueType::id, "name", &ValueType::name, "source_channel", &ValueType::sourceChannel, "source_guild", &ValueType::sourceGuild, "token",
			&ValueType::token, "type", &ValueType::type, "url", &ValueType::url, "user", &ValueType::user);
	};

	template<> struct core<discord_core_loader::AuditLogChangeData> {
		using ValueType					 = discord_core_loader::AuditLogChangeData;
		constexpr static auto parseValue = createValue("key", &ValueType::key, "new_value", &ValueType::newValue, "old_value", &ValueType::oldValue);
	};

	template<> struct core<discord_core_loader::OptionalAuditEntryInfoData> {
		using ValueType = discord_core_loader::OptionalAuditEntryInfoData;
		constexpr static auto parseValue =
			createValue("delete_member_days", &ValueType::deleteMemberDays, "members_removed", &ValueType::membersRemoved, "application_id", &ValueType::applicationId, "role_name",
				&ValueType::roleName, "channel_id", &ValueType::channelId, "message_id", &ValueType::messageId, "count", &ValueType::count, "type", &ValueType::type);
	};

	template<> struct core<discord_core_loader::ArchivedThreadsData> {
		using ValueType					 = discord_core_loader::ArchivedThreadsData;
		constexpr static auto parseValue = createValue("has_more", &ValueType::hasMore, "member", &ValueType::members, "threads", &ValueType::threads);
	};

	template<> struct core<discord_core_loader::ConnectionData> {
		using ValueType = discord_core_loader::ConnectionData;
		constexpr static auto parseValue =
			createValue("friend_sync", &ValueType::friendSync, "id", &ValueType::id, "integrations", &ValueType::integrations, "name", &ValueType::name, "revoked", &ValueType::revoked,
				"show_activity", &ValueType::showActivity, "type", &ValueType::type, "verified", &ValueType::verified, "visibility", &ValueType::visibility);
	};

	template<> struct core<discord_core_loader::GuildPruneCountData> {
		using ValueType					 = discord_core_loader::GuildPruneCountData;
		constexpr static auto parseValue = createValue("count", &ValueType::count);
	};

	template<> struct core<discord_core_loader::VoiceRegionData> {
		using ValueType = discord_core_loader::VoiceRegionData;
		constexpr static auto parseValue =
			createValue("custom", &ValueType::custom, "depracated", &ValueType::deprecated, "id", &ValueType::id, "name", &ValueType::name, "optimal", &ValueType::optimal);
	};

	template<> struct core<discord_core_loader::InviteData> {
		using ValueType					 = discord_core_loader::InviteData;
		constexpr static auto parseValue = createValue("approximate_member_count", &ValueType::approximateMemberCount, "approximate_presence_count",
			&ValueType::approximatePresenceCount, "channel", &ValueType::channel, "guild_id", &ValueType::guildId, "code", &ValueType::code, "created_at", &ValueType::createdAt,
			"expires_at", &ValueType::expiresAt, "guild", &ValueType::guild, "guild_scheduled_event", &ValueType::guildScheduledEvent, "inviter", &ValueType::inviter, "max_age",
			&ValueType::maxAge, "max_uses", &ValueType::maxUses, "stage_instance", &ValueType::stageInstance, "target_application", &ValueType::targetApplication, "target_type",
			&ValueType::targetType, "target_user", &ValueType::targetUser, "temporary", &ValueType::temporary, "uses", &ValueType::uses);
	};

	template<> struct core<discord_core_loader::ApplicationCommandData> {
		using ValueType					 = discord_core_loader::ApplicationCommandData;
		constexpr static auto parseValue = createValue("application_id", &ValueType::applicationId, "default_member_permissions", &ValueType::defaultMemberPermissions, "description",
			&ValueType::description, "guild_id", &ValueType::guildId, "description_localizations", &ValueType::descriptionLocalizations, "dm_permission", &ValueType::dmPermission,
			"id", &ValueType::id, "name", &ValueType::name, "name_localizations", &ValueType::nameLocalizations, "options", &ValueType::options, "type", &ValueType::type,
			"version", &ValueType::version);
	};

	template<> struct core<discord_core_loader::StickerData> {
		using ValueType					 = discord_core_loader::StickerData;
		constexpr static auto parseValue = createValue("asset", &ValueType::asset, "description", &ValueType::description, "format_type", &ValueType::formatType, "id", &ValueType::id,
			"name", &ValueType::name, "pack_id", &ValueType::packId, "tags", &ValueType::tags, "type", &ValueType::type, "user", &ValueType::user, "guild_id", &ValueType::guildId,
			"sort_value", &ValueType::sortValue, "nsfw_level", &ValueType::nsfwLevel);
	};

	template<> struct core<discord_core_loader::MessageActivityData> {
		using ValueType					 = discord_core_loader::MessageActivityData;
		constexpr static auto parseValue = createValue("type", &ValueType::type, "party_id", &ValueType::partyId);
	};

	template<> struct core<discord_core_loader::ActiveThreadsData> {
		using ValueType					 = discord_core_loader::ActiveThreadsData;
		constexpr static auto parseValue = createValue("has_more", &ValueType::hasMore, "members", &ValueType::members, "threads", &ValueType::threads);
	};

	template<> struct core<discord_core_loader::ActivityData> {
		using ValueType					 = discord_core_loader::ActivityData;
		constexpr static auto parseValue = createValue("name", &ValueType::name, "type", &ValueType::type, "url", &ValueType::url);
	};

	template<> struct core<discord_core_loader::PresenceUpdateData> {
		using ValueType					 = discord_core_loader::PresenceUpdateData;
		constexpr static auto parseValue = createValue("client_status", &ValueType::clientStatus, "guild_id", &ValueType::guildId, "status", &ValueType::status, "user",
			&ValueType::user, "activities", &ValueType::activities);
	};

	template<> struct core<discord_core_loader::AccountData> {
		using ValueType					 = discord_core_loader::AccountData;
		constexpr static auto parseValue = createValue("name", &ValueType::name);
	};

	template<> struct core<discord_core_loader::EmbedData> {
		using ValueType					 = discord_core_loader::EmbedData;
		constexpr static auto parseValue = createValue("author", &ValueType::author, "color", &ValueType::hexColorValue, "description", &ValueType::description, "fields",
			&ValueType::fields, "footer", &ValueType::footer, "image", &ValueType::image, "provider", &ValueType::provider, "thumbnail", &ValueType::thumbnail, "title",
			&ValueType::title, "type", &ValueType::type, "url", &ValueType::url, "video", &ValueType::video);
	};

	template<> struct core<discord_core_loader::MessageDataOld> {
		using ValueType					 = discord_core_loader::MessageDataOld;
		constexpr static auto parseValue = createValue("activity", &ValueType::activity, "application", &ValueType::application, "application_id", &ValueType::applicationId,
			"attachments", &ValueType::attachments, "author", &ValueType::author, "channel_id", &ValueType::channelId, "components", &ValueType::components, "content",
			&ValueType::content, "edited_timestamp", &ValueType::editedTimestamp, "embeds", &ValueType::embeds, "flags", &ValueType::flags, "guild_id", &ValueType::guildId, "id",
			&ValueType::id, "interaction", &ValueType::interaction, "mention_channels", &ValueType::mentionChannels, "mention_everyone", &ValueType::mentionEveryone,
			"mention_roles", &ValueType::mentionRoles, "mentions", &ValueType::mentions, "message_reference", &ValueType::messageReference, "nonce", &ValueType::nonce, "pinned",
			&ValueType::pinned, "reactions", &ValueType::reactions, "sticker_items", &ValueType::stickerItems, "stickers", &ValueType::stickers, "thread", &ValueType::thread,
			"tts", &ValueType::tts, "type", &ValueType::type);
	};

	template<> struct core<discord_core_loader::ReactionData> {
		using ValueType					 = discord_core_loader::ReactionData;
		constexpr static auto parseValue = createValue("channel_id", &ValueType::channelId, "count", &ValueType::count, "emoji", &ValueType::emoji, "guild_id", &ValueType::guildId,
			"id", &ValueType::id, "member", &ValueType::member, "me", &ValueType::me, "message_id", &ValueType::messageId, "user_id", &ValueType::userId);
	};

	template<> struct core<discord_core_loader::StickerItemData> {
		using ValueType					 = discord_core_loader::StickerItemData;
		constexpr static auto parseValue = createValue("format_type", &ValueType::formatType, "name", &ValueType::name);
	};

	template<> struct core<discord_core_loader::AttachmentData> {
		using ValueType					 = discord_core_loader::AttachmentData;
		constexpr static auto parseValue = createValue("content_type", &ValueType::contentType, "description", &ValueType::description, "filename", &ValueType::filename, "height",
			&ValueType::height, "id", &ValueType::id, "proxy_url", &ValueType::proxyUrl, "size", &ValueType::size, "url", &ValueType::url, "width", &ValueType::width, "ephemeral",
			&ValueType::ephemeral);
	};

	template<> struct core<discord_core_loader::ChannelMentionData> {
		using ValueType					 = discord_core_loader::ChannelMentionData;
		constexpr static auto parseValue = createValue("id", &ValueType::id, "guild_id", &ValueType::guildId, "name", &ValueType::name, "type", &ValueType::type);
	};

	template<> struct core<discord_core_loader::ActionRowData> {
		using ValueType					 = discord_core_loader::ActionRowData;
		constexpr static auto parseValue = createValue("components", &ValueType::components);
	};

	template<> struct core<discord_core_loader::EmbedFooterData> {
		using ValueType					 = discord_core_loader::EmbedFooterData;
		constexpr static auto parseValue = createValue("proxy_icon_url", &ValueType::proxyIconUrl, "icon_url", &ValueType::iconUrl, "text", &ValueType::text);
	};

	template<> struct core<discord_core_loader::EmbedImageData> {
		using ValueType					 = discord_core_loader::EmbedImageData;
		constexpr static auto parseValue = createValue("proxy_url", &ValueType::proxyUrl, "url", &ValueType::url, "height", &ValueType::height, "width", &ValueType::width);
	};

	template<> struct core<discord_core_loader::EmbedThumbnailData> {
		using ValueType					 = discord_core_loader::EmbedThumbnailData;
		constexpr static auto parseValue = createValue("proxy_url", &ValueType::proxyUrl, "url", &ValueType::url, "height", &ValueType::height, "width", &ValueType::width);
	};

	template<> struct core<discord_core_loader::EmbedVideoData> {
		using ValueType					 = discord_core_loader::EmbedVideoData;
		constexpr static auto parseValue = createValue("proxy_url", &ValueType::proxyUrl, "url", &ValueType::url, "height", &ValueType::height, "width", &ValueType::width);
	};

	template<> struct core<discord_core_loader::EmbedProviderData> {
		using ValueType					 = discord_core_loader::EmbedProviderData;
		constexpr static auto parseValue = createValue("name", &ValueType::name, "url", &ValueType::url);
	};

	template<> struct core<discord_core_loader::EmbedAuthorData> {
		using ValueType					 = discord_core_loader::EmbedAuthorData;
		constexpr static auto parseValue = createValue("proxy_icon_url", &ValueType::proxyIconUrl, "icon_url", &ValueType::iconUrl, "name", &ValueType::name, "url", &ValueType::url);
	};

	template<> struct core<discord_core_loader::EmbedFieldData> {
		using ValueType					 = discord_core_loader::EmbedFieldData;
		constexpr static auto parseValue = createValue("name", &ValueType::name, "value", &ValueType::value, "inline", &ValueType::Inline);
	};

	template<> struct core<discord_core_loader::AuditLogEntryData> {
		using ValueType					 = discord_core_loader::AuditLogEntryData;
		constexpr static auto parseValue = createValue("action_type", &ValueType::actionType, "changes", &ValueType::changes, "created_at", &ValueType::createdTimeStamp, "id",
			&ValueType::id, "options", &ValueType::options, "reason", &ValueType::reason, "target_id", &ValueType::targetId, "user_id", &ValueType::userId);
	};

	template<> struct core<discord_core_loader::ComponentData> {
		using ValueType					 = discord_core_loader::ComponentData;
		constexpr static auto parseValue = createValue("type", &ValueType::type, "custom_id", &ValueType::customId, "options", &ValueType::options, "placeholder",
			&ValueType::placeholder, "min_values", &ValueType::minValues, "max_values", &ValueType::maxValues, "min_length", &ValueType::minLength, "max_length",
			&ValueType::maxLength, "label", &ValueType::label, "value", &ValueType::value, "title", &ValueType::title, "emoji", &ValueType::emoji, "url", &ValueType::url,
			"required", &ValueType::required, "disabled", &ValueType::disabled, "style", &ValueType::style);
	};

	template<> struct core<discord_core_loader::SelectOptionData> {
		using ValueType = discord_core_loader::SelectOptionData;
		constexpr static auto parseValue =
			createValue("description", &ValueType::description, "label", &ValueType::label, "value", &ValueType::value, "emoji", &ValueType::emoji, "default", &ValueType::_default);
	};

	template<> struct core<discord_core_loader::ClientStatusData> {
		using ValueType					 = discord_core_loader::ClientStatusData;
		constexpr static auto parseValue = createValue("desktop", &ValueType::desktop, "mobile", &ValueType::mobile, "web", &ValueType::web);
	};

	template<> struct core<discord_core_loader::GuildData> {
		using ValueType					 = discord_core_loader::GuildData;
		constexpr static auto parseValue = createValue("id", &ValueType::id, "roles", &ValueType::roles, "channels", &ValueType::channels, "members", &ValueType::members);
	};

	template<> struct core<discord_core_loader::IntegrationData> {
		using ValueType					 = discord_core_loader::IntegrationData;
		constexpr static auto parseValue = createValue("account", &ValueType::account, "application", &ValueType::application, "enabled", &ValueType::enabled, "enable_emoticons",
			&ValueType::enableEmoticons, "expire_behavior", &ValueType::expireBehavior, "expire_grace_period", &ValueType::expireGracePeriod, "id", &ValueType::id, "name",
			&ValueType::name, "revoked", &ValueType::revoked, "role_id", &ValueType::roleId, "subscriber_count", &ValueType::subscriberCount, "syncing", &ValueType::syncing,
			"synced_at", &ValueType::syncedAt, "type", &ValueType::type, "user", &ValueType::user);
	};

	template<> struct core<discord_core_loader::File> {
		using ValueType					 = discord_core_loader::File;
		static constexpr auto parseValue = createScalarValue(&ValueType::data);
	};

	template<> struct core<discord_core_loader::AllowedMentionsData> {
		using ValueType					 = discord_core_loader::AllowedMentionsData;
		static constexpr auto parseValue = createValue("replied_user", &ValueType::repliedUser, "parse", &ValueType::parse, "roles", &ValueType::roles, "users", &ValueType::users);
	};

	template<> struct core<discord_core_loader::MessageReferenceData> {
		using ValueType = discord_core_loader::MessageReferenceData;
		constexpr static auto parseValue =
			createValue("fail_if_not_exists", &ValueType::failIfNotExists, "message_id", &ValueType::messageId, "channel_id", &ValueType::channelId, "guild_id", &ValueType::guildId);
	};

	template<> struct core<discord_core_loader::MessageInteractionData> {
		using ValueType					 = discord_core_loader::MessageInteractionData;
		constexpr static auto parseValue = createValue("id", &ValueType::id, "member", &ValueType::member, "name", &ValueType::name, "type", &ValueType::type, "user", &ValueType::user);
	};

	template<> struct core<discord_core_loader::GuildScheduledEventData> {
		using ValueType					 = discord_core_loader::GuildScheduledEventData;
		constexpr static auto parseValue = createValue("id", &ValueType::id, "privacy_level", &ValueType::privacyLevel, "entity_metadata", &ValueType::entityMetadata, "entity_type",
			&ValueType::entityType, "status", &ValueType::status, "scheduled_start_time", &ValueType::scheduledStartTime, "scheduled_end_time", &ValueType::scheduledEndTime,
			"description", &ValueType::description, "creator_id", &ValueType::creatorId, "channel_id", &ValueType::channelId, "entity_id", &ValueType::entityId, "user_count",
			&ValueType::userCount, "guild_id", &ValueType::guildId, "creator", &ValueType::creator, "name", &ValueType::name);
	};

	template<> struct core<discord_core_loader::GuildScheduledEventUserData> {
		using ValueType					 = discord_core_loader::GuildScheduledEventUserData;
		constexpr static auto parseValue = createValue("guild_scheduled_event_id", &ValueType::guildScheduledEventId, "member", &ValueType::member, "user", &ValueType::user);
	};

	template<> struct core<discord_core_loader::GuildScheduledEventMetadata> {
		using ValueType					 = discord_core_loader::GuildScheduledEventMetadata;
		constexpr static auto parseValue = createValue("location", &ValueType::location);
	};

	template<> struct core<discord_core_loader::RoleData> {
		using ValueType					 = discord_core_loader::RoleData;
		constexpr static auto parseValue = createValue("color", &ValueType::color, "flags", &ValueType::flags, "id", &ValueType::id, "name", &ValueType::name, "permissions",
			&ValueType::permissions, "position", &ValueType::position, "unicode_emoji", &ValueType::unicodeEmoji);
	};

	template<> struct core<discord_core_loader::StickerPackData> {
		using ValueType					 = discord_core_loader::StickerPackData;
		constexpr static auto parseValue = createValue("banner_asset_id", &ValueType::bannerAssetId, "cover_sticker_id", &ValueType::coverStickerId, "description",
			&ValueType::description, "name", &ValueType::name, "sku_id", &ValueType::skuId, "stickers", &ValueType::stickers);
	};

	template<> struct core<discord_core_loader::GuildWidgetImageData> {
		using ValueType					 = discord_core_loader::GuildWidgetImageData;
		constexpr static auto parseValue = createValue("url", &ValueType::url);
	};

	template<> struct core<discord_core_loader::GuildTemplateData> {
		using ValueType					 = discord_core_loader::GuildTemplateData;
		static constexpr auto parseValue = createValue("code", &ValueType::code, "created_at", &ValueType::createdAt, "creator", &ValueType::creator, "creator_id",
			&ValueType::creatorId, "description", &ValueType::description, "is_dirty", &ValueType::isDirty, "name", &ValueType::name, "serialized_source_guild",
			&ValueType::serializedSourceGuild, "source_guild_id", &ValueType::sourceGuildId, "updated_at", &ValueType::updatedAt);
	};

	template<> struct core<discord_core_loader::ApplicationCommandOptionData> {
		using ValueType					 = discord_core_loader::ApplicationCommandOptionData;
		constexpr static auto parseValue = createValue("type", &ValueType::type, "name", &ValueType::name, "description", &ValueType::description, "required", &ValueType::required,
			"autocomplete", &ValueType::autocomplete, "min_value", &ValueType::minValue, "max_value", &ValueType::maxValue, "channel_types", &ValueType::channelTypes,
			"description_localizations", &ValueType::descriptionLocalizations, "name_localizations", &ValueType::nameLocalizations, "choices", &ValueType::choices, "options",
			&ValueType::options);
	};

	template<> struct core<discord_core_loader::StageInstanceData> {
		using ValueType					 = discord_core_loader::StageInstanceData;
		constexpr static auto parseValue = createValue("channel_id", &ValueType::channelId, "discoverable_disabled", &ValueType::discoverableDisabled, "guild_id", &ValueType::guildId,
			"id", &ValueType::id, "privacy_level", &ValueType::privacyLevel, "topic", &ValueType::topic);
	};

	template<> struct core<discord_core_loader::ApplicationCommandOptionChoiceData> {
		using ValueType					 = discord_core_loader::ApplicationCommandOptionChoiceData;
		constexpr static auto parseValue = createValue("name", &ValueType::name, "value", &ValueType::value, "name_localized", &ValueType::nameLocalizations);
	};

	template<> struct core<discord_core_loader::RoleTagsData> {
		using ValueType					 = discord_core_loader::RoleTagsData;
		constexpr static auto parseValue = createValue("premium_subscriber", &ValueType::premiumSubscriber, "integration_id", &ValueType::integrationId, "bot_id", &ValueType::botId);
	};

	template<typename ValueTypeNew> struct core<discord_core_loader::WebSocketMessageReal<ValueTypeNew>> {
		using ValueType					 = discord_core_loader::WebSocketMessageReal<ValueTypeNew>;
		constexpr static auto parseValue = createValue("d", &ValueType::d, "t", &ValueType::t, "s", &ValueType::s, "op", &ValueType::op);
	};

	template<> struct core<discord_core_loader::WebSocketMessageReal<discord_core_loader::GuildData>> {
		using ValueType					 = discord_core_loader::WebSocketMessageReal<discord_core_loader::GuildData>;
		constexpr static auto parseValue = createValue("d", &ValueType::d, "t", &ValueType::t, "s", &ValueType::s, "op", &ValueType::op);
	};

	template<> struct core<discord_core_loader::WebSocketIdentifyData> {
		using ValueType = discord_core_loader::WebSocketIdentifyData;
		constexpr static auto parseValue = createValue("shard", &ValueType::shard, "intents", &ValueType::intents);
	};

	template<> struct core<discord_core_loader::WebSocketMessageData> {
		using ValueType = discord_core_loader::WebSocketMessageData;
		constexpr static auto parseValue = createValue("t", &ValueType::t, "s", &ValueType::s, "op", &ValueType::op);
	};

	template<> struct core<discord_core_loader::ConfigData> {
		using ValueType					 = discord_core_loader::ConfigData;
		constexpr static auto parseValue = createValue("ConnectionIp", &ValueType::connectionIp, "ConnectionPort", &ValueType::connectionPort,
			"DoWePrintWebSocketSuccessReceiveMessages", &ValueType::doWePrintWebSocketSuccessReceiveMessages, "DoWePrintWebSocketSuccessSentMessages",
			&ValueType::doWePrintWebSocketSuccessSentMessages, "DoWePrintWebSocketErrorMessages", &ValueType::doWePrintWebSocketErrorMessages,
			"DoWePrintGeneralSuccessMessages", &ValueType::doWePrintGeneralSuccessMessages, "DoWePrintGeneralErrorMessages",
			&ValueType::doWePrintGeneralErrorMessages, "GuildQuantity", &ValueType::guildQuantity, "StdDeviationForStringLength",
			&ValueType::stdDeviationForStringLength, "MeanForStringLength", &ValueType::meanForStringLength, "StdDeviationForMemberCount",
			&ValueType::stdDeviationForMemberCount, "MeanForMemberCount", &ValueType::meanForMemberCount, "StdDeviationForChannelCount",
			&ValueType::stdDeviationForChannelCount, "MeanForChannelCount", &ValueType::meanForChannelCount, "StdDeviationForRoleCount",
			&ValueType::stdDeviationForRoleCount, "MeanForRoleCount", &ValueType::meanForRoleCount, "ShardCount", &ValueType::shardCount);
	};


};// namespace discord_core_loader

/**@}*/
