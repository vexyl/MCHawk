ClassicProtocol = {
	ConnectEvent = 0,
	AuthEvent = 1,
	MessageEvent = 2,
	PositionEvent = 3,
	BlockEvent = 4,
	PluginLoadedEvent = 5
}

Flags = {
	NoDefaultCall = 0
}

this = "CorePlugin" -- must match table name below
CorePlugin = {}
CorePlugin.name = "core" -- should match folder name!

CorePlugin.pluginNames = {}

CorePlugin.init = function()
	Server.AddCommand("plugins", "plugin pl", CorePlugin.PluginsCommand, "&9/plugins - show information about plugins", 0, 0)
	Server.AddCommand("serverinfo", "sinfo server", CorePlugin.ServerInfoCommand, "&9/serverinfo - show information about the server", 0, 0)
	Server.AddCommand("reload", "", CorePlugin.ReloadCommand, "&9/reload - reloads server plugins", 0, 1)

	Server.RegisterEvent(ClassicProtocol.PluginLoadedEvent, CorePlugin.Plugins_OnPluginLoaded)
end

dofile("plugins/core/utils.lua")

include(CorePlugin, "core.lua")
LoadPlugin(CorePlugin, "permissions.lua")
