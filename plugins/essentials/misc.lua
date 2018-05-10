EssentialsPlugin.Emote_EmoteCommand = function(client, args)
	local message = "&9* " .. client.name
	message = message .. " " .. table.concat(args, " ")
	Server.BroadcastMessage(message)
end

EssentialsPlugin.Pm_PmCommand = function(client, args)
	local targetName = args[1]

	local target = Server.GetClientByName(targetName)
	if (target == nil) then
		Server.SendMessage(client, PLAYER_NOT_FOUND(targetName))
		return
	end

	if (string.lower(target.name) == string.lower(client.name)) then
		Server.SendMessage(client, "&cYou shouldn't be talking to yourself...")
		return
	end

	local message = table.concat(args, " ", 2) -- skip name argument

	Server.SendMessage(client, "&d[You -> " .. target.name .. "] " .. message)
	Server.SendMessage(target, "&d[" .. client.name .. "] " .. message)
end

-- Yes, this is an essential command
EssentialsPlugin.BillNye_BillNyeCommand = function(client, args)
	local message = table.concat(args, " ")

	Server.BroadcastMessage("&cBill Nye&f: DID YOU KNOW THAT")
	Server.BroadcastMessage("&cBill Nye&f: " .. message .. "?")
	Server.BroadcastMessage("&cBill Nye&f: NOW YOU KNOW!")

	-- Prevent client from spamming this command
	client:SetChatMute(1000)
end

EssentialsPlugin.Cmds_CmdsCommand = function(client, args)
	Server.SendMessage(client, "&eType &a/help <command> &efor more information on a command")

	local commandString = "&a" .. table.concat(Server.GetCommandStrings(), "&e, &a")
	Server.SendMessage(client, "&eCommands: " .. commandString)
end

EssentialsPlugin.Kick_KickCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "admin")) then
		return
	end

	if (args[1] == nil) then
		Server.SendMessage(client, "&cMust specify a player to kick")
		return
	end

	local targetName = args[1]

	local target = Server.GetClientByName(targetName)
	if (target == nil) then
		Server.SendMessage(client, PLAYER_NOT_FOUND(targetName))
		return
	end

	targetName = target.name;

	local message = client.name .. " kicked " .. targetName
	local reason = ""

	local index = 2 -- where reason should start
	if (args[index] ~= nil) then
		reason = table.concat(args, " ", index)
		message = message .. " (" .. reason .. ")"
	end

	Server.KickClient(target, reason)
	Server.SystemWideMessage(message)
end
