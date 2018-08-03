-- FIXME: Show invalid permissions as a comma-separated list in grant/revoke instead of sending a new line for each

this = "PermissionsPlugin" -- must match table name below
PermissionsPlugin = {}
PermissionsPlugin.name = "permissions" -- should match folder name!

PermissionsPlugin.permissionTable = {}
PermissionsPlugin.permissionList = {}

PermissionsPlugin.init = function()
	Server.AddCommand("grant", "", PermissionsPlugin.GrantCommand, "grant <player> <permission> - grants player permission", 2, 0)
	Server.AddCommand("revoke", "", PermissionsPlugin.RevokeCommand, "revoke <player> <permission> - revokes player permission", 2, 0)
	Server.AddCommand("permissions", "p perm perms", PermissionsPlugin.PermissionsCommand, "permissions [player] - shows player permissions", 0, 0)

	PermissionsPlugin.LoadPermissions()
	PermissionsPlugin.RequirePermission("permissions")

	print("Permissions plugin init")
end

PermissionsPlugin.GrantCommand = function(client, args, override)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "permissions") and override == nil) then
		return
	end

	local perms = table.concat(args, ", ", 2)

	local targetName = args[1]
	local target = Server.GetClientByName(targetName, false)

	if (target ~= nil) then
		targetName = target.name
	elseif (PermissionsPlugin.permissionTable[string.lower(targetName)] == nil) then
		Server.SendMessage(client, PLAYER_NOT_FOUND(targetName))
		return
	end

	if (not PermissionsPlugin.PermissionsExistsNotify(client, perms)) then
		return
	end

	for k, targetPerm in pairs(args) do
		if (k ~= 1) then -- skip name
			if (not PermissionsPlugin.CheckPermission(targetName, targetPerm)) then
				PermissionsPlugin.GrantPermission(targetName, targetPerm)
			end
		end
	end

	PermissionsPlugin.SavePermissions()

	Server.SendMessage(client, "&eGranted player " .. targetName ..": &9" .. perms)

	if (target ~= nil) then
		Server.SendMessage(target, "&e" .. client.name .. " granted you: &9" .. perms)
	end
end

PermissionsPlugin.RevokeCommand = function(client, args, override)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "permissions") and override == nil) then
		return
	end

	local perms = table.concat(args, ", ", 2)
	local targetName = args[1]
	local target = Server.GetClientByName(targetName, false)

	if (target ~= nil) then
		targetName = target.name
	elseif (PermissionsPlugin.permissionTable[string.lower(targetName)] == nil) then
		Server.SendMessage(client, PLAYER_NOT_FOUND(targetName))
		return
	end

	if (not PermissionsPlugin.PermissionsExistsNotify(client, perms)) then
		return
	end

	for k, targetPerm in pairs(args) do
		if (k ~= 1) then -- skip name
			if (PermissionsPlugin.CheckPermission(targetName, targetPerm)) then
				PermissionsPlugin.RevokePermission(targetName, targetPerm)
			end
		end
	end

	PermissionsPlugin.SavePermissions()

	Server.SendMessage(client, "&eRevoked player " .. targetName .. ": &9" .. perms)

	if (target ~= nil) then
		Server.SendMessage(target, "&e" .. client.name .. " revoked you: &9" .. perms)
	end
end

PermissionsPlugin.PermissionsCommand = function(client, args)
	local targetName = args[1]

	local target
	if (targetName == nil) then
		targetName = client.name
	else
		target = Server.GetClientByName(targetName, false)
		if (target ~= nil) then
			targetName = target.name
		end
	end

	local perms = PermissionsPlugin.permissionTable[string.lower(targetName)]
	local permissions = ""
	if (perms ~= nil) then
		permissions = "&9" .. table.concat(perms, "&e, &9")
	elseif (target == nil and args[1] ~= nil) then
		Server.SendMessage(client, PLAYER_NOT_FOUND(targetName))
		return
	end

	Server.SendMessage(client, "&ePermissions of " .. targetName .. ": " .. permissions)
end

-- Doesn't check if permission exists
PermissionsPlugin.GrantPermission = function(name, perm)
	local name = string.lower(name)

	local permsTable = PermissionsPlugin.permissionTable

	if (permsTable[name] == nil) then
		permsTable[name]= {}
	end

	table.insert(permsTable[name], perm)
end

-- Doesn't check if permission exists
PermissionsPlugin.RevokePermission = function(name, perm)
	local name = string.lower(name)

	local perms = PermissionsPlugin.permissionTable[name]

	if (#perms - 1 <= 0) then
		PermissionsPlugin.permissionTable[name] = nil
		return
	end

	for k, v in pairs(perms) do
		if (v == perm) then
			table.remove(perms, k)
			break
		end
	end
end

PermissionsPlugin.LoadPermissions = function()
	local f = io.open("permissions.txt", "r")
	if f then
		local lines = {}
		for line in io.lines("permissions.txt") do
			local tokens = split(line, ":")
			if (tokens == nil or tokens[1] == nil or tokens[2] == nil) then
				print("Permissions Plugin failed to load entry in permissions.txt")
				break
			end

			local perms = split(tokens[2], ", ")
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
				f:write(perm .. ", ")
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
		PermissionsPlugin.SendInvalidPermissionMessage(client, permission)
		return false
	end

	return true
end

-- Checks comma-separated list of permissions and returns false on first invalid permission
PermissionsPlugin.PermissionsExistsNotify = function(client, permissions)
	local result = true

	local perms = split(permissions, ", ")

	for _,perm in pairs(perms) do
		if (not PermissionsPlugin.PermissionExistsNotify(client, perm)) then
			result = false
		end
	end

	return result
end

-- Doesn't check if permission exists
PermissionsPlugin.CheckPermission = function(name, permission)
	local name = string.lower(name)

	local result = false

	local perms = PermissionsPlugin.permissionTable[name];
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

PermissionsPlugin.CheckPermissionIfExists = function(name, permission)
	if (not PermissionsPlugin.PermissionExists(permission)) then
		print("Permission plugin: invalid permission " .. permission .. " check in CheckPermissionIfExists")
		return false
	end

	return PermissionsPlugin.CheckPermission(name, permission)
end

PermissionsPlugin.CheckPermissionNotify = function(client, permission)
	if (not PermissionsPlugin.PermissionsExistsNotify(client, permission)) then
		return false
	end

	-- Operators bypass permissions
	if (client:GetUserType() >= 0x64) then -- operator or higher
		return true
	end

	local result = PermissionsPlugin.CheckPermission(client.name, permission)

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

