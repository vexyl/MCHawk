this = "EssentialsPlugin" -- must match table name below
EssentialsPlugin = {}
EssentialsPlugin.name = "essentials" -- should match folder name!

EssentialsPlugin.init = function()
	Server.AddCommand("shutdown", "", EssentialsPlugin.Server_ShutdownCommand, "&9/shutdown - shutdowns the server", 0, 1)
	Server.AddCommand("ban", "", EssentialsPlugin.Ban_BanCommand, "&9/ban <player> - bans player from server", 1, 1)
	Server.AddCommand("unban", "", EssentialsPlugin.Ban_UnbanCommand, "&9/unban <player> - unbans player from server", 1, 1)
	Server.AddCommand("cuboid", "z", EssentialsPlugin.Cuboid_CuboidCommand, "&9/cuboid - places blocks in a cuboid region", 0, 0)
	Server.AddCommand("emote", "me", EssentialsPlugin.Emote_EmoteCommand, "&9/emote <message> - unleashes an emote upon the world", 1, 0)
	Server.AddCommand("pm", "msg message whisper m", EssentialsPlugin.Pm_PmCommand, "&9/pm <name> <message> - sends a private message to a player", 2, 0)
	Server.AddCommand("billnye", "bn bill nye", EssentialsPlugin.BillNye_BillNyeCommand, "&9/billnye <wisdom> - instills wisdom in fellow server members", 1, 0)

	Server.RegisterEvent(ClassicProtocol.AuthEvent, EssentialsPlugin.Ban_OnAuth)
	Server.RegisterEvent(ClassicProtocol.BlockEvent, EssentialsPlugin.Cuboid_OnBlock)

	PermissionsPlugin.RequirePermission("server")
	PermissionsPlugin.RequirePermission("admin")
	PermissionsPlugin.RequirePermission("cuboid")
end

include (EssentialsPlugin, "server.lua")
include(EssentialsPlugin, "ban.lua")
include(EssentialsPlugin, "cuboid.lua")
include(EssentialsPlugin, "emote.lua")
include(EssentialsPlugin, "pm.lua")
include(EssentialsPlugin, "billnye.lua")
