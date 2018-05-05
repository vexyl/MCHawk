EssentialsPlugin.Emote_EmoteCommand = function(client, args)
	message = "&9* " .. client.name
	message = message .. " " .. table.concat(args, " ")
	Server.BroadcastMessage(message)
end

EssentialsPlugin.Pm_PmCommand = function(client, args)
	name = args[1]

	target = Server.GetClientByName(name)
	if (target == nil) then
		Server.SendMessage(client, "&cPlayer &f" .. name .. "&c does not exist")
		return
	end

	if (string.lower(target.name) == string.lower(client.name)) then
		Server.SendMessage(client, "&cYou shouldn't be talking to yourself...")
		return
	end

	message = table.concat(args, " ", 2) -- skip name argument

	Server.SendMessage(client, "&d[You -> " .. target.name .. "] " .. message)
	Server.SendMessage(target, "&d[" .. client.name .. "] " .. message)
end

-- Yes, this is an essential command
EssentialsPlugin.BillNye_BillNyeCommand = function(client, args)
	message = table.concat(args, " ")

	Server.BroadcastMessage("&cBill Nye&f: DID YOU KNOW THAT")
	Server.BroadcastMessage("&cBill Nye&f: " .. message .. "?")
	Server.BroadcastMessage("&cBill Nye&f: NOW YOU KNOW!")
end

EssentialsPlugin.Cmds_CmdsCommand = function(client, args)
	Server.SendMessage(client, "&eType &a/help <command> &efor more information on a command")

	commandString = "&a" .. table.concat(Server.GetCommandStrings(), "&e, &a")
	Server.SendMessage(client, "&eCommands: " .. commandString)
end
