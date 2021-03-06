EssentialsPlugin.Emote_EmoteCommand = function(client, args)
	local message = "&9* " .. client.name
	message = message .. " " .. table.concat(args, " ")
	Server.BroadcastMessage(message)
end

EssentialsPlugin.Pm_PmCommand = function(client, args)
	local targetName = args[1]
	local message = table.concat(args, " ", 2) -- skip name argument

	EssentialsPlugin.Pm_MessagePlayer(client, targetName, message)
end

EssentialsPlugin.Pm_OnMessage = function(client, args)
	local message = args["message"]

	if (string.sub(message, 1, 1) == "@") then
		local tokens = split(message)
		if (tokens[2] ~= nil) then
			local targetName = string.sub(tokens[1], 2, -1)
			local privateMessage = table.concat(tokens, " ", 2)

			EssentialsPlugin.Pm_MessagePlayer(client, targetName, privateMessage)

			Flags.NoDefaultCall = 1
		end
	end
end

EssentialsPlugin.Pm_MessagePlayer = function(client, targetName, message)
	local target = Server.GetClientByName(targetName)
	if (target == nil) then
		Server.SendMessage(client, PLAYER_NOT_FOUND(targetName))
		return
	end

	if (string.lower(target.name) == string.lower(client.name)) then
		Server.SendMessage(client, "&cYou shouldn't be talking to yourself...")
		return
	end

	Server.SendMessage(client, "&dTo [" .. target.name .. "]: " .. message)
	Server.SendMessage(target, "&d[" .. client.name .. "]: " .. message)
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
	if (not PermissionsPlugin.CheckPermissionNotify(client, "essentials.kick")) then
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

	local reason = "Kicked by " .. client.name

	local index = 2 -- where reason should start
	if (args[index] ~= nil) then
		reason = reason .. ": " .. table.concat(args, " ", index)
	end

	Server.KickClient(target, reason)
end

EssentialsPlugin.Misc_OnWorldJoin = function(client, args)
	local world = args["world"]
	local prevWorld = args["prev"]

	-- Only show message if warping from one world to another
	if (prevWorld ~= nil) then
		Server.BroadcastMessage(client:GetChatName() .. " &ewarped to &a" .. world:GetName())
	end
end

EssentialsPlugin.Misc_GotoCommand = function(client, args)
	local targetName = args[1]
	local world = Server.GetWorldByName(targetName)

	if (world == nil) then
		Server.SendMessage(client, WORLD_NOT_FOUND(targetName))
		return
	end

	-- Proper world name
	local worldName = world:GetName()

	-- inactive world
	if (not world:GetActive()) then
		Server.SendMessage(client, WORLD_NOT_ACTIVE(worldName))
		return
	end

	-- Same world player is in
	if (worldName == client:GetWorld():GetName()) then
		Server.SendMessage(client, "&eWarp nine. Engage. &9*Woosh*")
		return
	end

	Server.TransportPlayer(client, world)
end

EssentialsPlugin.Misc_WhoCommand = function(client, args)
	local message = "&ePlayer list: "

	local clients = Server.GetClients()
	for k,client in pairs(clients) do
		message = message .. client:GetChatName() .. "&e(&a" .. client:GetWorld():GetName() .. "&e)"
		if (k < #clients) then
			message = message .. ", "
		end
	end

	Server.SendMessage(client, message)
end

EssentialsPlugin.Misc_WhoIsCommand = function(client, args)
	local targetName = args[1]
	local target = Server.GetClientByName(targetName)
	if (target == nil) then
		Server.SendMessage(client, PLAYER_NOT_FOUND(targetName))
		return
	end

	local name = target.name

	Server.SendMessage(client, "&eWhoIs &f" .. name .. ":")
	Server.SendMessage(client, "&e------")
	Server.SendMessage(client, "&eChat name: &f" .. target:GetChatName())

	local groupMessage = "&eGroups: " .. table.concat(EssentialsPlugin.Groups_GetPlayerGroups(target), "&e, ")

	Server.SendMessage(client, groupMessage)

	if (PermissionsPlugin.CheckPermissionIfExists(client.name, "essentials.whois")) then
		local ipString = target:GetIpString()
		Server.SendMessage(client, "&eIP: &f" .. ipString)
	end
end
