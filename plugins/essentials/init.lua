this = "EssentialsPlugin" -- must match table name below
EssentialsPlugin = {}
EssentialsPlugin.name = "essentials" -- should match folder name!

EssentialsPlugin.init = function()
	Server.AddCommand("shutdown", "", EssentialsPlugin.Server_ShutdownCommand, "shutdown [-t <seconds>] [-a] [message] - shuts down the server", 0, 0)
	Server.AddCommand("ban", "", EssentialsPlugin.Ban_BanCommand, "ban <player> [reason] - bans player from server", 1, 0)
	Server.AddCommand("unban", "", EssentialsPlugin.Ban_UnbanCommand, "unban <player> - unbans player from server", 1, 0)
	Server.AddCommand("banip", "", EssentialsPlugin.Ban_BanIpCommand, "banip <ip address> [reason] - bans ip from server", 1, 0)
	Server.AddCommand("unbanip", "", EssentialsPlugin.Ban_UnbanIpCommand, "unbanip <ip address> - unbans ip from server", 1, 0)
	Server.AddCommand("cuboid", "z", EssentialsPlugin.Cuboid_CuboidCommand, "cuboid [air] - places blocks in a cuboid region", 0, 0)
	Server.AddCommand("emote", "me", EssentialsPlugin.Emote_EmoteCommand, "emote <message> - unleashes an emote upon the world", 1, 0)
	Server.AddCommand("pm", "msg message whisper", EssentialsPlugin.Pm_PmCommand, "pm <name> <message> - sends a private message to a player", 2, 0)
	Server.AddCommand("billnye", "bn bill nye", EssentialsPlugin.BillNye_BillNyeCommand, "billnye <wisdom> - instills wisdom in fellow server members", 1, 0)
	Server.AddCommand("cmds", "c cmd command commands", EssentialsPlugin.Cmds_CmdsCommand, "cmds - show short list of commands", 0, 0)
	Server.AddCommand("kick", "", EssentialsPlugin.Kick_KickCommand, "kick <player> [reason] - kicks player from server", 1, 0)
	Server.AddCommand("goto", "go g warp", EssentialsPlugin.Misc_GotoCommand, "goto <world name> - warps to world", 1, 0)
	Server.AddCommand("who", "player players list", EssentialsPlugin.Misc_WhoCommand, "who - gets player list", 0, 0)
	Server.AddCommand("whois", "pinfo playerinfo", EssentialsPlugin.Misc_WhoIsCommand, "whois <player> - gets information about player", 1, 0)

	local groupsCmd = Server.AddCommand("groups", "gr grp grps group rank ranks", EssentialsPlugin.Groups_GroupsCommand, "groups - show available permission groups", 0, 0)
	groupsCmd:AddSubcommand("reload", EssentialsPlugin.Groups_GroupsCommand_Reload, "reload - reads the groups file into memory", 0, 0)

	Server.AddCommand("main", "m def default",
		function(client, args)
			EssentialsPlugin.Misc_GotoCommand(client, { "default" })
		end,
	"main - warps to default world", 0, 0)

	Server.AddCommand("ragequit", "quit",
		function(client, args)
			local message = "invoked the sacred rights of RAGEQUIT"
			Server.KickClient(client, message)
		end,
	"ragequit - throw a fit and leave the server in tears", 0, 0)

	Server.RegisterEvent(ClassicProtocol.ConnectEvent, EssentialsPlugin.Ban_OnConnect)
	Server.RegisterEvent(ClassicProtocol.BlockEvent, EssentialsPlugin.Cuboid_OnBlock)
	Server.RegisterEvent(ClassicProtocol.DisconnectEvent, EssentialsPlugin.Cuboid_OnDisconnect)
	Server.RegisterEvent(ClassicProtocol.WorldJoinEvent, EssentialsPlugin.Cuboid_OnWorldJoin)
	Server.RegisterEvent(ClassicProtocol.MessageEvent, EssentialsPlugin.Pm_OnMessage)
	Server.RegisterEvent(ClassicProtocol.AuthEvent, EssentialsPlugin.Groups_OnAuth)
	Server.RegisterEvent(ClassicProtocol.WorldJoinEvent, EssentialsPlugin.Misc_OnWorldJoin)

	Server.RegisterEvent(ClassicProtocol.AuthEvent,
		function(client, args)
			Server.SendMessage(client, "https://github.com/vexyl/MCHawk")
			Server.SendMessage(client, "&eWelcome to this Minecraft classic server running " .. Server.GetName() .. "&e.")
			Server.SendMessage(client, "&eType &a/help &eor &a/goto freebuild &eto get started.")
		end
	)

	EssentialsPlugin.World.Init()

	PermissionsPlugin.RequirePermission("essentials.server")
	PermissionsPlugin.RequirePermission("essentials.ban")
	PermissionsPlugin.RequirePermission("essentials.kick")
	PermissionsPlugin.RequirePermission("essentials.cuboid")
	PermissionsPlugin.RequirePermission("essentials.groups")
	PermissionsPlugin.RequirePermission("essentials.whois")

	EssentialsPlugin.Ban_LoadBans()
	EssentialsPlugin.Groups_LoadGroups()
end

include(EssentialsPlugin, "server.lua")
include(EssentialsPlugin, "ban.lua")
include(EssentialsPlugin, "cuboid.lua")
include(EssentialsPlugin, "misc.lua")
include(EssentialsPlugin, "world.lua")
include(EssentialsPlugin, "groups.lua")
