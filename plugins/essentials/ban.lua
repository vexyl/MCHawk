-- TODO: Save ban reasons to file

EssentialsPlugin.Ban_banList = {}

EssentialsPlugin.Ban_BanCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "admin")) then
		return
	end

	local name = string.lower(args[1])

	local player = Server.GetClientByName(name, true)
	if (player == nil) then
		Server.SendMessage(client, "&cPlayer &f" .. name .. "&c not found")
		return
	end

	local message = "&e" .. client.name .. " has banned " .. name .. " from the server."
	local reason = ""

	if (args[2] ~= nil) then
		reason = table.concat(args, " ", 2) -- skip name argument
		message = message .. " (" .. reason .. ")"
	end

	Server.KickClient(player, "Banned: " .. reason)
	Server.BroadcastMessage(message)

	EssentialsPlugin.Ban_banList[name] = 1
	EssentialsPlugin.Ban_SaveBans()
end

EssentialsPlugin.Ban_UnbanCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "admin")) then
		return
	end

	local name = args[1]
	
	if (EssentialsPlugin.Ban_banList[name] == nil) then
		Server.SendMessage(client, "&cPlayer &f" .. name .. "&c not banned")
		return
	end

	EssentialsPlugin.Ban_banList[name] = nil
	EssentialsPlugin.Ban_SaveBans()

	Server.SendMessage(client, "&eUnbanned player " .. name)
end

EssentialsPlugin.Ban_OnAuth = function(client, args)
	-- Use args.name because client.name isn't set yet
	local name = string.lower(args.name)

	if (EssentialsPlugin.Ban_banList[name] ~= nil) then
		Server.KickClient(client, "Banned")
		print("Ban plugin prevented player " .. name .. " from joining.")
		Flags.NoDefaultCall = 1
	end
end

EssentialsPlugin.Ban_SaveBans = function()
	local f = io.open("bans.txt", "w")
	if f then
		for name,_ in pairs(EssentialsPlugin.Ban_banList) do
			f:write(name .. "\n")
		end

		f:close()
	end
end
EssentialsPlugin.Ban_LoadBans = function()
	local f = io.open("bans.txt", "r")
	if f then
		local lines = {}
		for line in io.lines("bans.txt") do
			EssentialsPlugin.Ban_banList[line] = 1
		end

		f:close()
	else
		print("Couldn't open bans.txt")
	end
end

EssentialsPlugin.Ban_LoadBans()
