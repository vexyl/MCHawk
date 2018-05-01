EssentialsPlugin.Cuboid_players = {}
EssentialsPlugin.Cuboid_count = {}
EssentialsPlugin.Cuboid_maxBlocks = 32 ^ 3
EssentialsPlugin.Cuboid_destroy = {}

EssentialsPlugin.Cuboid_CuboidCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "cuboid")) then
		return
	end

	world = client:GetWorld()
	if (world:GetOption("build") == "false") then
		Server.SendMessage(client, "&cCuboid disabled in no-build worlds")
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
		btype = Server.MapGetBlockType(client, block.x, block.y, block.z)
		Server.SendBlock(client, block.x, block.y, block.z, btype)
		
		Flags.NoDefaultCall = 1
	end
end

EssentialsPlugin.Cuboid_DoCuboid = function(client)
	blocks = EssentialsPlugin.Cuboid_players[client.name]

	x1 = blocks["1"].x
	y1 = blocks["1"].y
	z1 = blocks["1"].z

	x2 = blocks["2"].x
	y2 = blocks["2"].y
	z2 = blocks["2"].z

	btype = blocks["1"].type
	mode = blocks["1"].mode

	-- Set block type to air if player is used /z air
	if (EssentialsPlugin.Cuboid_destroy[client.name] == 1) then
		btype = 0
	end

	xstep = 1
	ystep = 1
	zstep = 1

	if (x1 > x2) then
		xstep = -1
	end

	if (y1 > y2) then
		ystep = -1
	end

	if (z1 > z2) then
		zstep = -1
	end

	dx = math.abs(x2 - x1) + 1
	dy = math.abs(y2 - y1) + 1
	dz = math.abs(z2 - z1) + 1

	blockCount = dx * dy * dz

	maxBlocks = EssentialsPlugin.Cuboid_maxBlocks
	if (blockCount > maxBlocks) then
		Server.SendMessage(client, "&cToo many blocks; Max=" .. maxBlocks)
		return
	end

	blocksPlaced = 0
	for x = x1, x2, xstep do
		for y = y1, y2, ystep do
			for z = z1, z2, zstep do
				Server.PlaceBlock(client, btype, x, y, z)
				blocksPlaced = blocksPlaced + 1
			end
		end
	end

	print("Placed " .. blocksPlaced .. " blocks (" .. blockCount .. ")")
	Server.SendMessage(client, "&ePlaced " .. blockCount .. " blocks")

	EssentialsPlugin.Cuboid_players[client.name] = nil
	EssentialsPlugin.Cuboid_count[client.name] = nil
	EssentialsPlugin.Cuboid_destroy[client.name] = nil
end

