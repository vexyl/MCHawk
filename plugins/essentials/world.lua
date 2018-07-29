EssentialsPlugin.World = {
Init = function()
	local worldCmd = Server.AddCommand("world", "w map", function() end, "world - various commands related to worlds", 1, 0)
	worldCmd:AddSubcommand("list", EssentialsPlugin.World.Command_List, "list - list all available worlds; active=green, inactive=red", 0, 0)
	worldCmd:AddSubcommand("set", EssentialsPlugin.World.Command_Set, "set [option] [value] - sets world options; leave off arguments to see list of options; leave off value to see current value", 0, 1)
	worldCmd:AddSubcommand("save", EssentialsPlugin.World.Command_Save, "save - saves world options and map data to file", 0, 1)
	worldCmd:AddSubcommand("load", EssentialsPlugin.World.Command_Load, "load <world name> - loads world map into memory", 1, 0)
	worldCmd:AddSubcommand("new", EssentialsPlugin.World.Command_New, "new <world name> <x> <y> <z>", 4, 0)

	Server.AddCommand("worlds", "", function(client, args) EssentialsPlugin.World.Command_List(client, args) end, "worlds - shortcut for /world list", 0, 0)

	Server.RegisterEvent(ClassicProtocol.BlockEvent, EssentialsPlugin.World.Event_OnBlock)

	PermissionsPlugin.RequirePermission("essentials.world")

	local worlds = Server.GetWorlds()
	for _,world in pairs(worlds) do
		PermissionsPlugin.RequirePermission(world:GetName() .. ".build")
	end
end,

Event_OnBlock = function(client, block)
	if (not client:CanBuild() and not PermissionsPlugin.CheckPermissionNotify(client, client:GetWorld():GetName() .. ".build")) then
		-- reverse block change client-side
		local btype = Server.MapGetBlockType(client, block.x, block.y, block.z)
		Server.SendBlock(client, block.x, block.y, block.z, btype)
		
		Flags.NoDefaultCall = 1
	end
end,

Command_List = function(client, args)
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

Command_Set = function(client, args)
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

Command_Save = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "essentials.world")) then
		return
	end

	local world = client:GetWorld()
	world:Save()
	Server.SendMessage(client, "&eWorld saved")
end,

Command_Load = function(client, args)
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

Command_New = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "essentials.world")) then
		return
	end

	local worldName = string.lower(args[1])

	if (string.len(worldName) > 10) then
		Server.SendMessage(client, "&cInvalid name")
		return
	end

	local x = tonumber(args[2])
	local y = tonumber(args[3])
	local z = tonumber(args[4])

	if (type(x) ~= 'number' or type(y) ~= 'number' or type(z) ~= 'number') then
		Server.SendMessage(client, "&cMalformed input")
		return
	end

	if ((x > 512 or x <= 0) or (y > 64 or y <=0) or (z > 512 or z <= 0)) then
		Server.SendMessage(client, "&cInvalid size. Max map size is &f512&cx&f64&cx&f512&c.")
		return
	end

	if (Server.GetWorldByName(worldName) ~= nil) then
		Server.SendMessage(client, WORLD_ALREADY_EXISTS(worldName))
		return
	end

	Server.CreateWorld(worldName, x, y, z)

	Server.SendMessage(client, "&eWorld '" .. worldName .. "' with size " .. x .. "x" .. y .. "x" .. z .. " created successfully.")
end
}
