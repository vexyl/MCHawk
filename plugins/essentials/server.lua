-- TODO: Add an optional shutdown timer
EssentialsPlugin.Server_ShutdownCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "server")) then
		return
	end

	name = "CONSOLE"
	if (client ~= nil) then
		name = client.name
	end

	message = "(" .. name .. ")" .. " Server shutting down..."
	reason = "Server shut down"

	if (args[1] ~= nil) then
		reason = table.concat(args, " ")
		message = message .. " (" .. reason .. ")"
	end
 
	Server.SystemWideMessage(message)
	EssentialsPlugin.Server_SafeShutdown(reason);
end

EssentialsPlugin.Server_SafeShutdown = function(reason)
	clients = Server.GetClients()
	for _,client in pairs(clients) do
		Server.SendKick(client, reason)
	end

	worlds = Server.GetWorlds()
	for _,world in pairs(worlds) do
		if (world:GetOption("autosave") == "true") then
			world:Save()
		end
	end

	Server.Shutdown()
end
