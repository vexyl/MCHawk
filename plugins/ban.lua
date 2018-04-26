this = "BanPlugin"

BanPlugin = {}

BanPlugin.init = function()
	AddCommand("ban", BanPlugin.BanCommand, "&9/ban <player> - bans player from server", 1, 1)
	AddCommand("unban", BanPlugin.UnbanCommand, "&9/unban <player> - unbans player from server", 1, 1)

	RegisterEvent(ClassicProtocol.AuthEvent, BanPlugin.OnAuth)
end

BanPlugin.BanCommand = function(client, args)
	if (not PermissionPlugin.CheckPermissionNotify(client, "ban")) then
		return
	end

	name = args[1]

	local f = io.open("bans.txt", "a")
	if f then
		player = Server.GetClientByName(name)
		if (player == nil) then
			Server.SendMessage(client, "&cPlayer " .. name .. " not found")
			f:close()
			return
		end

		f:write(string.lower(name) .. "\n")
		Server.KickClient(player, "Banned")
		Server.BroadcastMessage("&e" .. client.name .. " has banned " .. name .. " from the server.")

		f:close()
	end
end

BanPlugin.UnbanCommand = function(client, args)
	if (not PermissionPlugin.CheckPermissionNotify(client, "ban")) then
		return
	end

	Server.SendMessage(client, "&eNot yet")
end

BanPlugin.OnAuth = function(client, args)
	local f = io.open("bans.txt", "r")
	if f then
		lines = {}
		for line in io.lines("bans.txt") do
			-- Use table.name because client.name isn't set yet
			if (line == string.lower(args.name)) then
				Server.KickClient(client, "Banned")
				Flags.NoDefaultCall = 1
			end
		end

		f:close()
	else
		print("Couldn't open bans.txt")
	end
end
