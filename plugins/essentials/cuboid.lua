EssentialsPlugin.players = {}
EssentialsPlugin.count = {}
EssentialsPlugin.Cuboid_maxBlocks = 32 ^ 3

EssentialsPlugin.CuboidCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "admin")) then
		return
	end

	Server.SendMessage(client, "&eMake two selections")
	EssentialsPlugin.players[client.name] = {}
	EssentialsPlugin.count[client.name] = 0
end

EssentialsPlugin.Cuboid_OnBlock = function(client, block)
	for k, v in pairs(EssentialsPlugin.players) do
		if (EssentialsPlugin.players[client.name] ~= nil) then
			EssentialsPlugin.count[client.name] = EssentialsPlugin.count[client.name] + 1

			if (EssentialsPlugin.count[client.name] == 1) then
				EssentialsPlugin.players[client.name]["1"] = block
			elseif (EssentialsPlugin.count[client.name] == 2) then
				EssentialsPlugin.players[client.name]["2"] = block

				EssentialsPlugin.DoCuboid(client)
			end

			-- reverse block change client-side
			btype = Server.MapGetBlockType(client, block.x, block.y, block.z)
			Server.SendBlock(client, block.x, block.y, block.z, btype)
		end
	end
end

EssentialsPlugin.DoCuboid = function(client)
	blocks = EssentialsPlugin.players[client.name]

	x1 = blocks["1"].x
	y1 = blocks["1"].y
	z1 = blocks["1"].z

	x2 = blocks["2"].x
	y2 = blocks["2"].y
	z2 = blocks["2"].z

	btype = blocks["1"].type
	mode = blocks["1"].mode

	-- Set block type to air if player is breaking a block
	if (mode == 0) then
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
		Server.SendMessage(client, "&cToo many blocks. Max=" .. maxBlocks)
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

	EssentialsPlugin.players[client.name] = nil
	EssentialsPlugin.count[client.name] = nil
end
