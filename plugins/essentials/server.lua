EssentialsPlugin.Server_ShutdownCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "server")) then
		return
	end

	local index = 1 -- used to concatenate reason message

	local time = 15
	if (args[1] == "-t" and args[2] ~= nil) then
		time = tonumber(args[2])
		index = 3 -- skip time arguments
	elseif (args[1] == "-a") then
		if (CorePlugin.TimerExists("server_timers")) then
			CorePlugin.RemoveTimer("server_timers")
			Server.SystemWideMessage(client.name .. " aborted server shutdown.")
		else
			Server.SendMessage(client, "&cNo shutdown to abort")
		end

		return
	end

	local message = "(" .. client.name .. ")" .. " Server shutting down in " .. time .. " seconds..."
	local reason = "Server shut down"

	if (args[index] ~= nil) then
		reason = table.concat(args, " ", index)
		message = message .. " (" .. reason .. ")"
	end
 
	Server.SystemWideMessage(message)

	if (time > 10) then
		CorePlugin.AddTimer("server_timers", function() Server.SystemWideMessage("Server shutting down in 10 seconds!") end, time - 10)
	end

	if (time > 5) then
		CorePlugin.AddTimer("server_timers", function() Server.SystemWideMessage("Server shutting down in 5 seconds!") end, time - 5)
	end

	CorePlugin.AddTimer("server_timers", function() EssentialsPlugin.Server_SafeShutdown(reason) end, time)
end

EssentialsPlugin.Server_SafeShutdown = function(reason)
	local clients = Server.GetClients()
	for _,client in pairs(clients) do
		Server.SendKick(client, reason)
	end

	local worlds = Server.GetWorlds()
	for _,world in pairs(worlds) do
		if (world:GetOption("autosave") == "true") then
			world:Save()
		end
	end

	Server.Shutdown()
end
