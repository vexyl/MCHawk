-- TODO: Grant/Revoke comma-separated permissions, send message to granted/revoked player

this = "PermissionsPlugin" -- must match table name below
PermissionsPlugin = {}
PermissionsPlugin.name = "permissions" -- should match folder name!

PermissionsPlugin.permissionTable = {}
PermissionsPlugin.permissionList = {}

PermissionsPlugin.init = function()
	Server.AddCommand("grant", "", PermissionsPlugin.GrantCommand, "&9/grant <player> <permission> - grants player permission", 2, 0)
	Server.AddCommand("revoke", "", PermissionsPlugin.RevokeCommand, "&9/revoke <player> <permission> - revokes player permission", 2, 0)
	Server.AddCommand("permissions", "p perm perms", PermissionsPlugin.PermissionsCommand, "&9/permissions [player] - shows player permissions", 0, 0)

	PermissionsPlugin.LoadPermissions()
	PermissionsPlugin.RequirePermission("permissions")

	print("Permissions plugin init")
end

PermissionsPlugin.GrantCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "permissions")) then
		return
	end

	targetName = args[1]
	targetPerm = args[2]
	target = Server.GetClientByName(targetName, false)

	if (target == nil) then
		Server.SendMessage(client, "&cPlayer " .. targetName .. " not found")
		return
	end

	if (not PermissionsPlugin.PermissionExistsNotify(client, targetPerm)) then
		return false
	end

	if (not PermissionsPlugin.CheckPermission(target.name, targetPerm)) then
		permsTable = PermissionsPlugin.permissionTable

		if (permsTable[target.name] == nil) then
			permsTable[target.name]= {}
		end

		table.insert(permsTable[target.name], targetPerm)
		PermissionsPlugin.SavePermissions()
	end

	Server.SendMessage(client, "&eGranted player " .. target.name ..": &9" .. targetPerm)
	Server.SendMessage(target, "&e" .. client.name .. " granted you: &9" .. targetPerm)
end

PermissionsPlugin.RevokeCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "permissions")) then
		return
	end

	targetName = args[1]
	targetPerm = args[2]
	target = Server.GetClientByName(targetName, false)

	if (target == nil) then
		Server.SendMessage(client, "&cPlayer " .. targetName .. " not found")
		return
	end

	if (not PermissionsPlugin.PermissionExistsNotify(client, targetPerm)) then
		return false
	end

	if (PermissionsPlugin.CheckPermission(target.name, targetPerm)) then
		perms = PermissionsPlugin.permissionTable[target.name]

		for k, perm in pairs(perms) do
			if (perm == targetPerm) then
				table.remove(perms, k)
				PermissionsPlugin.SavePermissions()
				break
			end
		end
	end

	Server.SendMessage(client, "&eRevoked player " .. target.name ..": &9" .. targetPerm)
	Server.SendMessage(target, "&e" .. client.name .. " revoked you: &9" .. targetPerm)
end

PermissionsPlugin.PermissionsCommand = function(client, args)
	name = args[1]
	target = client

	if (name ~= nil) then
		target = Server.GetClientByName(name, false)
		if (target == nil) then
			Server.SendMessage(client, "&cPlayer " .. name .. " not found")
			return
		end
	end

	perms = PermissionsPlugin.permissionTable[target.name]

	permissions = ""
	if (perms ~= nil) then
		for k, perm in pairs(perms) do
			permissions = permissions .. "&9" .. perm .. "&e, "
		end
	end

	Server.SendMessage(client, "&ePermissions: " .. permissions)
end

PermissionsPlugin.LoadPermissions = function()
	local f = io.open("permissions.txt", "r")
	if f then
		lines = {}
		for line in io.lines("permissions.txt") do
			tokens = split(line, ":")
			if (tokens == nil or tokens[1] == nil or tokens[2] == nil) then
				print("Permissions Plugin failed to load entry in permissions.txt")
				break
			end

			perms = split(tokens[2], ",")
			PermissionsPlugin.permissionTable[tokens[1]] = perms
		end

		f:close()
	end
end

PermissionsPlugin.SavePermissions = function()
	local f = io.open("permissions.txt", "w")
	if f then
		for name in pairs(PermissionsPlugin.permissionTable) do
			f:write(name .. ":")

			for k, perm in pairs(PermissionsPlugin.permissionTable[name]) do
				f:write(perm .. ",")
			end

			f:write("\n")
		end

		f:close()
	end
end

PermissionsPlugin.RequirePermission = function(permission)
	table.insert(PermissionsPlugin.permissionList, permission)
	print("PermissionsPlugin new permission: " .. permission)
end

PermissionsPlugin.PermissionExists = function(permission)
	for k, perm in pairs(PermissionsPlugin.permissionList) do
		if (perm == permission) then
			return true
		end
	end

	return false
end

PermissionsPlugin.PermissionExistsNotify = function(client, permission)
	if (not PermissionsPlugin.PermissionExists(permission)) then
		Server.SendMessage(client, "&cInvalid permission " .. "&9" .. permission)
		return false
	end

	return true
end

-- TODO: Allow operators to override permissions: if (Server.IsOperator(name)) then return true end
-- Doesn't check if permission exists
PermissionsPlugin.CheckPermission = function(name, permission)
	result = false

	perms = PermissionsPlugin.permissionTable[name];
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

PermissionsPlugin.CheckPermissionNotify = function(client, permission)
	if (not PermissionsPlugin.PermissionExistsNotify(client, permission)) then
		return false
	end

	result = PermissionsPlugin.CheckPermission(client.name, permission)

	if (not result) then
		PermissionsPlugin.SendNoPermissionMessage(client, permission)
	end

	return result
end

PermissionsPlugin.SendInvalidPermissionMessage = function(client, permission)
	Server.SendMessage(client, "&cInvalid permission: &9" .. permission)
end

PermissionsPlugin.SendNoPermissionMessage = function(client, permission)
	Server.SendMessage(client, "&cMissing permissions: &9" .. permission)
end
