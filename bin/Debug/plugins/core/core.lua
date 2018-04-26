pluginCount = 0
pluginNames = { }

Plugin = {
	name = "Core",

	init = function()
		AddCommand("plugins", Plugin.PluginsCommand, "&9/plugins - show information about plugins", 0, 0)
		AddCommand("serverinfo", Plugin.ServerInfoCommand, "&9/serverinfo - show information about the server", 0, 0)
		AddCommand("reload", Plugin.ReloadCommand, "&9/reload - reloads server plugins", 0, 1)

		RegisterEvent(ClassicProtocol.PluginLoadedEvent, Plugin.OnPluginLoaded)
	end,

	OnPluginLoaded = function(client, args)
		pluginCount = pluginCount + 1
		table.insert(pluginNames, args.name)
	end,

	PluginsCommand = function(client, args)
		Server.SendMessage(client, "&eLuaBridge/" .. _VERSION)
		Server.SendMessage(client, "&e" .. pluginCount .. " plugin(s) loaded")
		Server.SendMessage(client, "&ePlugins: &9" .. table.concat(pluginNames, "&e, &9"))
		-- show plugins
	end,

	ServerInfoCommand = function(client, args)
		Server.SendMessage(client, "&eNot yet")
	end,

	ReloadCommand = function(client, args)
		Server.SendMessage(client, "&eNot yet")
	end
}
