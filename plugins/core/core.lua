CorePlugin.OnPluginLoaded = function(client, args)
	CorePlugin.pluginCount = CorePlugin.pluginCount + 1
	table.insert(CorePlugin.pluginNames, args.name)
end

CorePlugin.PluginsCommand = function(client, args)
	Server.SendMessage(client, "&eLuaBridge/" .. _VERSION)
	Server.SendMessage(client, "&e" .. CorePlugin.pluginCount .. " plugin(s) loaded")
	Server.SendMessage(client, "&ePlugins: &9" .. table.concat(CorePlugin.pluginNames, "&e, &9"))
end

CorePlugin.ServerInfoCommand = function(client, args)
	Server.SendMessage(client, "&eNot yet")
end

CorePlugin.ReloadCommand = function(client, args)
	Server.SendMessage(client, "&eNot yet")
end
