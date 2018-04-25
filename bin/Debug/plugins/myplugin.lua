Plugin = {
	name = "Test",

	init = function()
		RegisterEvent(ClassicProtocol.MessageEvent, Plugin.OnMessage)
		RegisterEvent(ClassicProtocol.AuthEvent, Plugin.OnAuth)
	end,

	OnMessage = function(client, table)
		print(client.name .. ": " .. table.message)
		if (string.match(table.message, "fuck")) then
			print("BAD WORD DETECTED!!!")
			Server.KickClient(client, "Bad word")
			Server.BroadcastMessage("Tsk tsk, " .. client.name .. ".");
		end
	end,

	OnAuth = function(client, table)
		local f = io.open("bans.txt", "r")
		if f then
			lines = {}
			for line in io.lines("bans.txt") do
				if (string.match(line, client.name)) then
					Server.KickClient(client, "Access denied: BANNED");
					print("Denied banned player " .. client.name)
				end
			end

			f:close()
		else
			print("Couldn't open bans.txt")
		end
	end
}
