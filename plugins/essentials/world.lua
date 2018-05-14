EssentialsPlugin.World_WorldCommand = function(client, args)

end

EssentialsPlugin.World_WorldCommand_List = function(client, args)
	local message = "&eWorlds: "
	local worlds = Server.GetWorlds()

	local length = GetTableLength(worlds)

	for index,world in ipairs(worlds) do
		if (world:GetActive()) then
			message = message .. "&a"
		else
			message = message .. "&c"
		end

		message = message .. world:GetName()

		if (index < length) then
			message = message .. "&e, "
		end
	end

	Server.SendMessage(client, message)
end
