EssentialsPlugin.players = {}
EssentialsPlugin.count = {}

EssentialsPlugin.CuboidCommand = function(client, args)
	if (not PermissionsPlugin.CheckPermissionNotify(client, "admin")) then
		return
	end

	Server.SendMessage(client, "&eSelect two regions.")
	EssentialsPlugin.players[client.name] = {}
	EssentialsPlugin.count[client.name] = 0
end

EssentialsPlugin.Cuboid_OnBlock = function(client, block)
	for k, v in pairs(EssentialsPlugin.players) do
		if (EssentialsPlugin.players[client.name] ~= nil) then
			EssentialsPlugin.count[client.name] = EssentialsPlugin.count[client.name] + 1

			Server.SendMessage(client, "&eSelection " .. EssentialsPlugin.count[client.name])
			-- TODO: Send block back
			if (EssentialsPlugin.count[client.name] == 1) then
				EssentialsPlugin.players[client.name]["1"] = block
			elseif (EssentialsPlugin.count[client.name] == 2) then
				EssentialsPlugin.players[client.name]["2"] = block

				Server.SendMessage(client, "&eOK")
				EssentialsPlugin.DoCuboid(client)
			end
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

	blocksPlaced = 0
	for x = x1, x2, xstep do
		for y = y1, y2, ystep do
			for z = z1, z2, zstep do
				PlaceBlock(client, btype, x, y, z)
				blocksPlaced = blocksPlaced + 1
			end
		end
	end

	Server.SendMessage(client, "&ePlaced " .. blocksPlaced .. " blocks")

	EssentialsPlugin.players[client.name] = nil
	EssentialsPlugin.count[client.name] = nil
end
