this = "PermissionPlugin"

PermissionPlugin = {}

PermissionPlugin.permissionTable = {}

PermissionPlugin.init = function()
	AddCommand("grant", PermissionPlugin.GrantCommand, "&9/grant <player> <permission> - grants player permission", 2, 0)
	AddCommand("revoke", PermissionPlugin.RevokeCommand, "&9/revoke <player> <permission> - revokes player permission", 2, 0)
	AddCommand("permissions", PermissionPlugin.PermissionsCommand, "&9/permissions [player] - shows player permissions", 0, 0)

	PermissionPlugin.LoadPermissions()
	PermissionPlugin.SavePermissions()

	print("Permissions plugin init")
end

PermissionPlugin.GrantCommand = function(client, args)
	if (not PermissionPlugin.CheckPermissionNotify(client, "permission")) then
		return
	end

	targetName = args[1]
	targetPerm = args[2]
	target = Server.GetClientByName(targetName)

	if (target == nil) then
		Server.SendMessage(client, "&cPlayer " .. targetName .. " not found")
		return
	end

	if (not PermissionPlugin.CheckPermission(target.name, targetPerm)) then
		perms = PermissionPlugin.permissionTable[target.name]
		table.insert(perms, targetPerm)
		PermissionPlugin.SavePermissions()
	end

	Server.SendMessage(client, "&eGranted player " .. target.name ..": &9" .. targetPerm)
end

PermissionPlugin.RevokeCommand = function(client, args)
	if (not PermissionPlugin.CheckPermissionNotify(client, "permission")) then
		return
	end

	targetName = args[1]
	targetPerm = args[2]
	target = Server.GetClientByName(targetName)

	if (target == nil) then
		Server.SendMessage(client, "&cPlayer " .. targetName .. " not found")
		return
	end

	if (PermissionPlugin.CheckPermission(target.name, targetPerm)) then
		perms = PermissionPlugin.permissionTable[target.name]

		for k, perm in pairs(perms) do
			if (perm == targetPerm) then
				table.remove(perms, k)
				PermissionPlugin.SavePermissions()
				break
			end
		end
	end

	Server.SendMessage(client, "&eRevoked player " .. target.name ..": &9" .. targetPerm)
end

PermissionPlugin.PermissionsCommand = function(client, args)
	name = args[1]
	target = client

	if (name ~= nil) then
		target = Server.GetClientByName(name)
		if (target == nil) then
			Server.SendMessage(client, "&cPlayer " .. name .. " not found")
			return
		end
	end

	perms = PermissionPlugin.permissionTable[target.name]

	permissions = ""
	if (perms ~= nil) then
		for k, perm in pairs(perms) do
			permissions = permissions .. "&9" .. perm .. "&e, "
		end
	end

	Server.SendMessage(target, "&ePermissions: " .. permissions)
end

PermissionPlugin.LoadPermissions = function()
	local f = io.open("permissions.txt", "r")
	if f then
		lines = {}
		for line in io.lines("permissions.txt") do
			tokens = mysplit(line, ":")
			if (tokens == nil or tokens[1] == nil or tokens[2] == nil) then
				print("Permissions Plugin failed to load entry in permissions.txt")
				break
			end

			perms = mysplit(tokens[2], ",")
			PermissionPlugin.permissionTable[tokens[1]] = perms
		end

		f:close()
	end
end

PermissionPlugin.SavePermissions = function()
	local f = io.open("permissions.txt", "w")
	if f then
		for name in pairs(PermissionPlugin.permissionTable) do
			f:write(name .. ":")

			for k, perm in pairs(PermissionPlugin.permissionTable[name]) do
				f:write(perm .. ",")
			end

			f:write("\n")
		end

		f:close()
	end
end

PermissionPlugin.CheckPermission = function(name, permission)
	result = false

	perms = PermissionPlugin.permissionTable[name];
	if (perms == nil) then
		result = false
	else
		for k, v in pairs(perms) do
			if (v == permission) then
				result = true
				break
			end
		end
	end

	return result
end

PermissionPlugin.CheckPermissionNotify = function(client, permission)
	result = PermissionPlugin.CheckPermission(client.name, permission)

	if (not result) then
		PermissionPlugin.SendNoPermissionMessage(client, permission)
	end

	return result
end

PermissionPlugin.SendNoPermissionMessage = function(client, permission)
	Server.SendMessage(client, "&cMissing permissions: &9" .. permission)
end
