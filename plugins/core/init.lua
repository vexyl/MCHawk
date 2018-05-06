ClassicProtocol = {
	ConnectEvent = 0,
	AuthEvent = 1,
	MessageEvent = 2,
	PositionEvent = 3,
	BlockEvent = 4,
	PluginLoadedEvent = 5,
	DisconnectEvent = 6
}

Flags = {
	NoDefaultCall = 0
}

this = "CorePlugin" -- must match table name below
CorePlugin = {}
CorePlugin.name = "core" -- should match folder name!

CorePlugin.pluginNames = {}
CorePlugin.timerList = {}

CorePlugin.init = function()
	Server.AddCommand("plugins", "plugin pl", CorePlugin.PluginsCommand, "&9/plugins - show information about plugins", 0, 0)
	Server.AddCommand("serverinfo", "sinfo server", CorePlugin.ServerInfoCommand, "&9/serverinfo - show information about the server", 0, 0)
	Server.AddCommand("reload", "", CorePlugin.ReloadCommand, "&9/reload - reloads server plugins", 0, 1)

	Server.RegisterEvent(ClassicProtocol.PluginLoadedEvent, CorePlugin.Plugins_OnPluginLoaded)
end

CorePlugin.tick = function()
	for timerName in pairs(CorePlugin.timerList) do
		t = CorePlugin.timerList[timerName]
		for k,v in pairs(t) do
			a,b = coroutine.resume(v[1], v[2])

			if (a == false or b == nil) then
				t[k] = nil
			end
		end
	end
end

dofile("plugins/" .. CorePlugin.name .. "/utils.lua")

include(CorePlugin, "core.lua")
LoadPlugin(CorePlugin, "permissions.lua")
