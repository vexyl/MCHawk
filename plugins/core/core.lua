this = "CorePlugin"

CorePlugin = {}

CorePlugin.pluginCount = 0
CorePlugin.pluginNames = {}

CorePlugin.init = function()
	AddCommand("plugins", CorePlugin.PluginsCommand, "&9/plugins - show information about plugins", 0, 0)
	AddCommand("serverinfo", CorePlugin.ServerInfoCommand, "&9/serverinfo - show information about the server", 0, 0)
	AddCommand("reload", CorePlugin.ReloadCommand, "&9/reload - reloads server plugins", 0, 1)

	RegisterEvent(ClassicProtocol.PluginLoadedEvent, CorePlugin.OnPluginLoaded)
end

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

function mysplit(inputstr, sep)
if sep == nil then
        sep = "%s"
end
local t={} ; i=1
for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
        t[i] = str
        i = i + 1
end
return t
end
