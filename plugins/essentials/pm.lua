EssentialsPlugin.PmCommand = function(client, args)
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
