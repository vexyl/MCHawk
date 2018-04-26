Plugin = {
	name = "Ban",

	init = function()
		AddCommand("ban", Plugin.BanCommand, "&9/ban <player> - bans player from server", 1, 1)
		AddCommand("unban", Plugin.UnbanCommand, "&9/unban <player> - unbans player from server", 1, 1)

		RegisterEvent(ClassicProtocol.AuthEvent, Plugin.OnAuth)
	end,

	BanCommand = function(client, args)
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
	end,

	UnbanCommand = function(client, args)
		Server.SendMessage(client, "&eNot yet")
	end,

	OnAuth = function(client, table)
		local f = io.open("bans.txt", "r")
		if f then
			lines = {}
			for line in io.lines("bans.txt") do
				-- Use table.name because client.name isn't set yet
				if (line == string.lower(table.name)) then
					Server.KickClient(client, "Banned")
					Flags.NoDefaultCall = 1
				end
			end

			f:close()
		else
			print("Couldn't open bans.txt")
		end
	end
}
