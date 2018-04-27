this = "EssentialsPlugin" -- must match table name below
EssentialsPlugin = {}
EssentialsPlugin.name = "essentials" -- should match folder name!

include(EssentialsPlugin, "ban.lua")

EssentialsPlugin.init = function()
	AddCommand("ban", "", EssentialsPlugin.BanCommand, "&9/ban <player> - bans player from server", 1, 1)
	AddCommand("unban", "", EssentialsPlugin.UnbanCommand, "&9/unban <player> - unbans player from server", 1, 1)

	RegisterEvent(ClassicProtocol.AuthEvent, EssentialsPlugin.Ban_OnAuth)

	PermissionsPlugin.RequirePermission("ban")
end
