-- FIXME: Show invalid permissions as a comma-separated list in grant/revoke instead of sending a new line for each

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

	local perms = table.concat(args, ", ", 2)

	local targetName = args[1]
	local target = Server.GetClientByName(targetName, false)

	if (target == nil) then
		Server.SendMessage(client, "&cPlayer &f" .. targetName .. "&c not found")
		return
	end

	if (not PermissionsPlugin.PermissionsExistsNotify(client, perms)) then
		return
	end

	for k, targetPerm in pairs(args) do
		if (k ~= 1) then -- skip name
			if (not PermissionsPlugin.CheckPermission(target.name, targetPerm)) then
				PermissionsPlugin.GrantPermission(target.name, targetPerm)
			end
		end
	end

	PermissionsPlugin.SavePermissions()

	Server.SendMessage(client, "&eGranted player " .. target.name ..": &9" .. perms)
	Server.SendMessage(target, "&e" .. client.name .. " granted you: &9" .. perms)
end

PermissionsPlugin.RevokeCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "permissions")) then
		return
	end

	local perms = table.concat(args, ", ", 2)
	local targetName = args[1]
	local target = Server.GetClientByName(targetName, false)

	if (target == nil) then
		Server.SendMessage(client, "&cPlayer &f" .. targetName .. "&c not found")
		return
	end

	if (not PermissionsPlugin.PermissionsExistsNotify(client, perms)) then
		return
	end

	for k, targetPerm in pairs(args) do
		if (k ~= 1) then -- skip name
			if (PermissionsPlugin.CheckPermission(target.name, targetPerm)) then
				PermissionsPlugin.RevokePermission(target.name, targetPerm)
			end
		end
	end

	PermissionsPlugin.SavePermissions()

	Server.SendMessage(client, "&eRevoked player " .. target.name ..": &9" .. perms)
	Server.SendMessage(target, "&e" .. client.name .. " revoked you: &9" .. perms)
end

PermissionsPlugin.PermissionsCommand = function(client, args)
	local name = args[1]
	local target = client

	if (name ~= nil) then
		target = Server.GetClientByName(name, false)
		if (target == nil) then
			Server.SendMessage(client, "&cPlayer &f" .. name .. "&c not found")
			return
		end
	end

	local perms = PermissionsPlugin.permissionTable[target.name]

	local permissions = ""
	if (perms ~= nil) then
		for k, perm in pairs(perms) do
			permissions = permissions .. "&9" .. perm .. "&e, "
		end
	end

	Server.SendMessage(client, "&ePermissions of " .. target.name .. ": " .. permissions)
end

-- Doesn't check if permission exists
PermissionsPlugin.GrantPermission = function(name, perm)
	local permsTable = PermissionsPlugin.permissionTable

	if (permsTable[name] == nil) then
		permsTable[name]= {}
	end

	table.insert(permsTable[name], perm)
end

-- Doesn't check if permission exists
PermissionsPlugin.RevokePermission = function(name, perm)
	local perms = PermissionsPlugin.permissionTable[name]

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

	perms = split(permissions, ", ")

	for _,perm in pairs(perms) do
		if (not PermissionsPlugin.PermissionExistsNotify(client, perm)) then
			result = false
		end
	end

	return result
end

-- TODO: Allow operators to override permissions: if (Server.IsOperator(name)) then return true end
-- Doesn't check if permission exists
PermissionsPlugin.CheckPermission = function(name, permission)
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

PermissionsPlugin.CheckPermissionNotify = function(client, permission)
	if (not PermissionsPlugin.PermissionsExistsNotify(client, permission)) then
		return false
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
