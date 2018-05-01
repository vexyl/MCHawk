CorePlugin.Plugins_OnPluginLoaded = function(client, args)
	table.insert(CorePlugin.pluginNames, args.name)
end

CorePlugin.PluginsCommand = function(client, args)
	Server.SendMessage(client, "&eLuaBridge/" .. _VERSION)
	Server.SendMessage(client, "&e" .. CorePlugin.GetPluginCount() .. " plugin(s) loaded")
	Server.SendMessage(client, "&ePlugins: &9" .. table.concat(CorePlugin.pluginNames, "&e, &9"))
end

CorePlugin.ServerInfoCommand = function(client, args)
	Server.SendMessage(client, "&eNot yet")
end

CorePlugin.ReloadCommand = function(client, args)
	Server.SendMessage(client, "&eNot yet")
end

CorePlugin.GetPluginCount = function()
	return GetTableLength(CorePlugin.pluginNames)
end

CorePlugin.AddTimer = function(name, func, time)
	co = coroutine.create(timer)
	coroutine.resume(co, time, func)

	if (CorePlugin.timerList[name] == nil) then
		CorePlugin.timerList[name] = {}
	end

	table.insert(CorePlugin.timerList[name], { co, time })
end

CorePlugin.RemoveTimer = function(name)
	CorePlugin.timerList[name] = nil
end

CorePlugin.TimerExists = function(name)
	if (CorePlugin.timerList[name] == nil) then
		return false
	end

	return true
end
