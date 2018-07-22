EssentialsPlugin.World = {
Init = function()
	local worldCmd = Server.AddCommand("luaworld", "lw", function() end, "world - various commands related to worlds", 1, 0)
	local worldListCmd = worldCmd:AddSubcommand("list", EssentialsPlugin.World.WorldCommand_List, "list - list all available worlds; active=green, inactive=red", 0, 0)
	worldCmd:AddSubcommand("set", EssentialsPlugin.World.WorldCommand_Set, "set [option] [value] - sets world options; leave off arguments to see list of options; leave off value to see current value", 0, 1)
	worldCmd:AddSubcommand("save", EssentialsPlugin.World.WorldCommand_Save, "save - saves world options and map data to file", 0, 1)
	worldCmd:AddSubcommand("load", EssentialsPlugin.World.WorldCommand_Load, "load <world name> - loads world map into memory", 1, 0)
	worldCmd:AddSubcommand("new", EssentialsPlugin.World.WorldCommand_New, "new <world name> <x> <y> <z>", 4, 0)

	Server.AddCommand("worlds", "", function(client, args) EssentialsPlugin.World.WorldCommand_List(client, args) end, "worlds - shortcut for /world list", 0, 0)

	PermissionsPlugin.RequirePermission("essentials.world")
end,

WorldCommand_List = function(client, args)
	local message = "&eWorlds: "
	local worlds = Server.GetWorlds()

	for index,world in ipairs(worlds) do
		if (world:GetActive()) then
			message = message .. "&a"
		else
			message = message .. "&c"
		end

		message = message .. world:GetName()

		if (index < #worlds) then
			message = message .. "&e, "
		end
	end

	Server.SendMessage(client, message)
end,

WorldCommand_Set = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "essentials.world")) then
		return
	end

	local world = client:GetWorld()

	if (#args == 0) then
		local options = World.GetOptionNames(world)
		local message = table.concat(options, ", ")

		Server.SendMessage(client, "&eOptions: " .. message)

		return
	end

	local option = args[1]

	if (#args == 1) then
		local value = world:GetOption(option)
		if (value ~= nil) then
			Server.SendMessage(client, "&e" .. option .. "=" .. value)
		else
			Server.SendMessage(client, "&cInvalid option")
		end

		return
	end

	local value = args[2]

	if (value ~= "true" and value ~= "false") then
		Server.SendMessage(client, "&cInvalid value")
		return
	end

	world:SetOption(option, value)	
	Server.SendMessage(client, "&eSet option " .. option .. " to " .. value)
end,

WorldCommand_Save = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "essentials.world")) then
		return
	end

	local world = client:GetWorld()
	world:Save()
	Server.SendMessage(client, "&eWorld saved")
end,

WorldCommand_Load = function(client, args)
	local targetName = string.lower(args[1])

	local targetWorld = Server.GetWorldByName(targetName)
	if (targetWorld == nil) then
		Server.SendMessage(client, WORLD_NOT_FOUND(targetName))
		return
	end

	local worldName = targetWorld:GetName()
	if (targetWorld:GetActive()) then
		Server.SendMessage(client, "&cWorld &a" .. worldName .. " &calready loaded")
		return
	end

	targetWorld:GetMap():Load()
	targetWorld:SetActive(true)

	Server.SendMessage(client, "&eLoaded world &a" .. worldName)
end,

WorldCommand_New = function(client, args)
	Server.SendMessage(client, "&cNot yet implemented")
end
}
