EssentialsPlugin.Cuboid_players = {}
EssentialsPlugin.Cuboid_count = {}
EssentialsPlugin.Cuboid_maxBlocks = 32 ^ 3
EssentialsPlugin.Cuboid_destroy = {}

EssentialsPlugin.Cuboid_CuboidCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "essentials.cuboid")) then
		return
	end

	if (EssentialsPlugin.Cuboid_players[client.name] ~= nil) then
		EssentialsPlugin.Cuboid_Remove(client.name)
		Server.SendMessage(client, "&eCuboid operation canceled")
		return
	end

	Server.SendMessage(client, "&eMake two selections")

	EssentialsPlugin.Cuboid_players[client.name] = {}
	EssentialsPlugin.Cuboid_count[client.name] = 0

	if (args[1] == "air") then
		EssentialsPlugin.Cuboid_destroy[client.name] = 1
	end
end

EssentialsPlugin.Cuboid_OnBlock = function(client, block)
	if (EssentialsPlugin.Cuboid_players[client.name] ~= nil) then
		EssentialsPlugin.Cuboid_count[client.name] = EssentialsPlugin.Cuboid_count[client.name] + 1

		if (EssentialsPlugin.Cuboid_count[client.name] == 1) then
			EssentialsPlugin.Cuboid_players[client.name]["1"] = block
		elseif (EssentialsPlugin.Cuboid_count[client.name] == 2) then
			EssentialsPlugin.Cuboid_players[client.name]["2"] = block

			EssentialsPlugin.Cuboid_DoCuboid(client)
		end

		-- reverse block change client-side
		local btype = Server.MapGetBlockType(client, block.x, block.y, block.z)
		Server.SendBlock(client, block.x, block.y, block.z, btype)
		
		Flags.NoDefaultCall = 1
	end
end

EssentialsPlugin.Cuboid_OnDisconnect = function(client, args)
	if (EssentialsPlugin.Cuboid_players[client.name] ~= nil) then
		EssentialsPlugin.Cuboid_Remove(client.name)
	end
end

EssentialsPlugin.Cuboid_OnWorldJoin = function(client, args)
	if (EssentialsPlugin.Cuboid_players[client.name] ~= nil) then
		EssentialsPlugin.Cuboid_Remove(client.name)
	end
end

EssentialsPlugin.Cuboid_DoCuboid = function(client)
	local world = client:GetWorld()

	if (client:CanBuild()) then
		local blocks = EssentialsPlugin.Cuboid_players[client.name]

		local x1 = blocks["1"].x
		local y1 = blocks["1"].y
		local z1 = blocks["1"].z

		local x2 = blocks["2"].x
		local y2 = blocks["2"].y
		local z2 = blocks["2"].z

		local btype = blocks["1"].type
		local mode = blocks["1"].mode

		-- Set block type to air if player is used /z air
		if (EssentialsPlugin.Cuboid_destroy[client.name] == 1) then
			btype = 0
		end

		local xstep = 1
		local ystep = 1
		local zstep = 1

		if (x1 > x2) then
			xstep = -1
		end

		if (y1 > y2) then
			ystep = -1
		end

		if (z1 > z2) then
			zstep = -1
		end

		local dx = math.abs(x2 - x1) + 1
		local dy = math.abs(y2 - y1) + 1
		local dz = math.abs(z2 - z1) + 1

		local blockCount = dx * dy * dz

		local maxBlocks = EssentialsPlugin.Cuboid_maxBlocks
		if (blockCount > maxBlocks) then
			Server.SendMessage(client, "&cToo many blocks; Max=" .. maxBlocks)
			return
		end

		for x = x1, x2, xstep do
			for y = y1, y2, ystep do
				for z = z1, z2, zstep do
					Server.PlaceBlock(client, btype, x, y, z)
				end
			end
		end

		Server.SendMessage(client, "&ePlaced " .. blockCount .. " blocks")
	else
		-- No building here
		Server.SendMessage(client, "&cCuboid disabled in no-build worlds")
	end

	-- Cleanup
	EssentialsPlugin.Cuboid_Remove(client.name)
end

EssentialsPlugin.Cuboid_Remove = function(name)
	EssentialsPlugin.Cuboid_players[name] = nil
	EssentialsPlugin.Cuboid_count[name] = nil
	EssentialsPlugin.Cuboid_destroy[name] = nil
end
