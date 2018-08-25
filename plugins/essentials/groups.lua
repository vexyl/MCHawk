EssentialsPlugin.Groups_groupsTable = {}
EssentialsPlugin.Groups_playerTable = {}

EssentialsPlugin.Groups_GroupsCommand = function(client, args)
	Server.SendMessage(client, "&f---&eGroups&f---")
	for _,groupTable in ipairs(EssentialsPlugin.Groups_groupsTable) do
		local group = groupTable[1]
		local color = groupTable[2]
		Server.SendMessage(client, "| " .. color .. group)
	end
end

EssentialsPlugin.Groups_GroupsCommand_Reload = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "essentials.groups")) then
		return
	end

	EssentialsPlugin.Groups_groupsTable = {}
	EssentialsPlugin.Groups_playerTable = {}

	EssentialsPlugin.Groups_LoadGroups()

	Server.SendMessage(client, "&eReloaded groups")

	-- hack to reload current players
	for _,v in pairs(Server.GetClients()) do
		EssentialsPlugin.Groups_OnAuth(v, nil)
	end
end

EssentialsPlugin.Groups_OnAuth = function(client, args)
	local targetName = string.lower(client.name)
	local groups = EssentialsPlugin.Groups_playerTable[targetName]

	if (groups == nil) then
		groups = { "guest" }
	end

	for k,group in ipairs(groups) do
		local groupTable = EssentialsPlugin.Groups_GetGroupTable(group)

		local color = groupTable[2]
		local perms = groupTable[3]

		if (k == 1) then -- first group set chat name with color
			client:SetChatName(color .. client.name)
		end

		if (perms ~= nil) then
			for _,perm in pairs(perms) do
				if (not PermissionsPlugin.HasPermission(targetName, perm)) then
					PermissionsPlugin.GrantPermission(targetName, perm)
					Server.SendMessage(client, "&eConsole granted you &9" .. perm)
				end
			end
		end
	end
end

EssentialsPlugin.Groups_GetGroupTable = function(group)
	for _,groupTable in pairs(EssentialsPlugin.Groups_groupsTable) do
		if (group == groupTable[1]) then
			return groupTable
		end
	end

	return nil
end

EssentialsPlugin.Groups_GetPlayerGroups = function(client)
	local targetName = string.lower(client.name)
	local groups = EssentialsPlugin.Groups_playerTable[targetName]

	if (groups == nil) then
		groups = { "guest" }
	end

	local groupStrings = {}
	for _,group in ipairs(groups) do
		local groupTable = EssentialsPlugin.Groups_GetGroupTable(group)
		local color = groupTable[2]

		table.insert(groupStrings, color .. group)
	end

	return groupStrings
end

EssentialsPlugin.Groups_LoadGroups = function()
	local f = io.open("groups.txt", "r")
	if f then
		local lines = {}
		for line in io.lines("groups.txt") do
			local tokens = split(line, ":")
			if (tokens == nil or tokens[1] == nil or tokens[2] == nil) then
				print("Groups Plugin failed to load entry in groups.txt")
				break
			end

			local action = tokens[1]
			if (action == "group") then
				local group = tokens[2]
				local color = tokens[3]
				local perms = nil
				local validGroup = true

				if (tokens[4] ~= nil) then
					perms = split(tokens[4], ", ")

					for _,perm in pairs(perms) do
						if (not PermissionsPlugin.PermissionExists(perm)) then
							print("Group plugin: invalid permission for " .. group .. ": " .. perm)
							validGroup = false
							break
						end
					end
				end

				if (validGroup) then
					table.insert(EssentialsPlugin.Groups_groupsTable, { group, color, perms })
				end
			elseif (action == "name") then
				-- grant player permissions
				local name = tokens[2]
				local groups = split(tokens[3], ", ")

				local validGroup = true
				for _,group in pairs(groups) do
					local groupTable = EssentialsPlugin.Groups_GetGroupTable(group)

					if (groupTable == nil) then
						print("Group plugin: invalid group for " .. name .. ": " .. group)
						validGroup = false
						break
					end
				end

				if (validGroup) then
					EssentialsPlugin.Groups_playerTable[name] = groups
				end
			end
		end

		f:close()
	else
		print("Couldn't open groups.txt")
	end
	
	local guestTable = EssentialsPlugin.Groups_GetGroupTable("guest")
	if (guestTable == nil) then
		table.insert(EssentialsPlugin.Groups_groupsTable, { "guest", "&7", nil })
	end
end
