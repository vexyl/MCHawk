EssentialsPlugin.Ban_banList = {}
EssentialsPlugin.Ban_ipBanList = {}

EssentialsPlugin.Ban_BanCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "admin")) then
		return
	end

	local name = string.lower(args[1])

	local player = Server.GetClientByName(name, true)
	if (player == nil) then
		Server.SendMessage(client, PLAYER_NOT_FOUND(name))
		return
	end

	local message1 = "Banned"
	local message2 = "&e" .. client.name .. " has banned " .. name .. " from the server."
	local reason = nil

	if (args[2] ~= nil) then
		reason = table.concat(args, " ", 2) -- skip name argument
		reason = reason:gsub(":+", "") -- strip semicolons as it is used to delimit ban lists
		message1 = message1 .. ": " .. reason
		message2 = message2 .. " (" .. reason .. ")"
	end

	Server.KickClient(player, message1)
	Server.BroadcastMessage(message2)

	EssentialsPlugin.Ban_banList[name] = { reason }
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

EssentialsPlugin.Ban_BanIpCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "admin")) then
		return
	end

	local ip = args[1]
	local reason = nil

	if (args[2] ~= nil) then
		reason = table.concat(args, " ", 2) -- skip name argument
		reason = reason:gsub(":+", "") -- strip semicolons as it is used to delimit ban lists
	end

	local target = nil

	local clients = Server.GetClients()
	for _,v in pairs(clients) do
		if (v:GetIpString() == ip) then
			local message1 = "Banned"
			local message2 = "&e" .. client.name .. " IP banned " .. v.name

			if (reason ~= nil) then
				message1 = message1 .. ": " .. reason
				message2 = message2 .. " (" .. reason .. ")"
			end

			Server.KickClient(v, message1)
			EssentialsPlugin.Ban_banList[name] = { reason } -- ban name too
			Server.SystemWideMessage(message2)

			break;
		end
	end

	Server.SendMessage(client, "&eYou IP banned " .. ip)

	EssentialsPlugin.Ban_ipBanList[ip] = { reason }
	EssentialsPlugin.Ban_SaveBans()
end

EssentialsPlugin.Ban_UnbanIpCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "admin")) then
		return
	end

	local ip = args[1]
	
	if (EssentialsPlugin.Ban_ipBanList[ip] == nil) then
		Server.SendMessage(client, "&cIP &f" .. ip .. "&c not banned")
		return
	end

	EssentialsPlugin.Ban_ipBanList[ip] = nil
	EssentialsPlugin.Ban_SaveBans()

	Server.SendMessage(client, "&eUnbanned IP " .. ip)
end

EssentialsPlugin.Ban_OnAuth = function(client, args)
	-- Use args.name because client.name isn't set yet
	local name = string.lower(args.name)
	local ip = client:GetIpString()
	local reason = nil
	local isBanned = false

	if (EssentialsPlugin.Ban_banList[name] ~= nil) then
		if (EssentialsPlugin.Ban_banList[name][1] ~= nil) then
			reason = EssentialsPlugin.Ban_banList[name][1]
		end

		isBanned = true
	elseif (EssentialsPlugin.Ban_ipBanList[ip] ~= nil) then
		if (EssentialsPlugin.Ban_ipBanList[ip][1] ~= nil) then
			reason = EssentialsPlugin.Ban_ipBanList[ip][1]
		end

		isBanned = true
	end

	if (isBanned) then
		local message = "Banned"

		if (reason ~= nil) then
			message = message .. ": " .. reason
		end

		Server.KickClient(client, message)
		print("Ban plugin prevented player " .. name .. " from joining.")
		Flags.NoDefaultCall = 1
	end
end

EssentialsPlugin.Ban_SaveBans = function()
	local f = io.open("bans.txt", "w")
	if f then
		for name,t in pairs(EssentialsPlugin.Ban_banList) do
			line = "name:" .. name

			if (t[1] ~= nil) then
				line = line .. ":" .. t[1]
			end

			f:write(line .. "\n")
		end

		for ip,t in pairs(EssentialsPlugin.Ban_ipBanList) do
			line = "ip:" .. ip

			if (t[1] ~= nil) then
				line = line .. ":" .. t[1]
			end

			f:write(line .. "\n")
		end

		f:close()
	end
end
EssentialsPlugin.Ban_LoadBans = function()
	local f = io.open("bans.txt", "r")
	if f then
		local lines = {}
		for line in io.lines("bans.txt") do
			local tokens = split(line, ":")
			local banType = tokens[1]
			local reason = nil

			if (GetTableLength(tokens) == 3) then
				reason = tokens[3]
			end

			if (banType == "name") then
				local name = tokens[2]
				EssentialsPlugin.Ban_banList[name] = { reason }
			elseif (banType == "ip") then
				local ip = tokens[2]
				EssentialsPlugin.Ban_ipBanList[ip] = { reason }
			end
		end

		f:close()
	else
		print("Couldn't open bans.txt")
	end
end

EssentialsPlugin.Ban_LoadBans()
