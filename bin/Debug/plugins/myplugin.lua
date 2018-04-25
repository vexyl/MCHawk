Plugin = {
	name = "Test",

	init = function()
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
				print("name=" .. table.name .. " line=" .. line)
				if (string.match(line, table.name)) then
					Server.KickClient(client, "Access denied: BANNED");
					print("Denied banned player " .. table.name)
					Flags.NoDefaultCall = 1;
				end
			end

			f:close()
		else
			print("Couldn't open bans.txt")
		end
	end
}
