Plugin = {
	name = "Test",

	init = function()
		AddCommand("ban", Plugin.BanCommand, "/ban <player>", 1, 1)

		RegisterEvent(ClassicProtocol.MessageEvent, Plugin.OnMessage)
		RegisterEvent(ClassicProtocol.AuthEvent, Plugin.OnAuth)
	end,

	OnMessage = function(client, table)
		print(client.name .. ": " .. table.message)
	end,

	OnAuth = function(client, table)
		local f = io.open("bans.txt", "r")
		if f then
			lines = {}
			for line in io.lines("bans.txt") do
				-- Use table.name because client.name isn't set yet
				if (line == string.lower(table.name)) then
					Server.KickClient(client, "Access denied: BANNED")
					print("Denied banned player " .. table.name)
					Flags.NoDefaultCall = 1
				end
			end

			f:close()
		else
			print("Couldn't open bans.txt")
		end
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
			Server.BroadcastMessage("&ePlayer " .. name .. " banned from server.")

			f:close()
		end
	end
}
