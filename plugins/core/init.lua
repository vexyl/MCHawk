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

dofile("plugins/core/utils.lua")

this = "CorePlugin" -- must match table name below
CorePlugin = {}
CorePlugin.name = "core" -- should match folder name!

CorePlugin.pluginCount = 0
CorePlugin.pluginNames = {}

CorePlugin.init = function()
	AddCommand("plugins", "", CorePlugin.PluginsCommand, "&9/plugins - show information about plugins", 0, 0)
	AddCommand("serverinfo", "sinfo server", CorePlugin.ServerInfoCommand, "&9/serverinfo - show information about the server", 0, 0)
	AddCommand("reload", "", CorePlugin.ReloadCommand, "&9/reload - reloads server plugins", 0, 1)

	RegisterEvent(ClassicProtocol.PluginLoadedEvent, CorePlugin.OnPluginLoaded)
end

include(CorePlugin, "core.lua")
include(CorePlugin, "permissions.lua")
