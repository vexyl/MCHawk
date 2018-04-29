this = "EssentialsPlugin" -- must match table name below
EssentialsPlugin = {}
EssentialsPlugin.name = "essentials" -- should match folder name!

EssentialsPlugin.init = function()
	Server.AddCommand("ban", "", EssentialsPlugin.BanCommand, "&9/ban <player> - bans player from server", 1, 1)
	Server.AddCommand("unban", "", EssentialsPlugin.UnbanCommand, "&9/unban <player> - unbans player from server", 1, 1)
	Server.AddCommand("cuboid", "z", EssentialsPlugin.CuboidCommand, "&9/cuboid - places blocks in a cuboid region", 0, 0)

	Server.RegisterEvent(ClassicProtocol.AuthEvent, EssentialsPlugin.Ban_OnAuth)
	Server.RegisterEvent(ClassicProtocol.BlockEvent, EssentialsPlugin.Cuboid_OnBlock)

	PermissionsPlugin.RequirePermission("admin")
end

include(EssentialsPlugin, "ban.lua")
include(EssentialsPlugin, "cuboid.lua")
