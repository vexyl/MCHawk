EssentialsPlugin.EmoteCommand = function(client, args)
	message = "&9* " .. client.name
	
	for _, v in pairs(args) do
		message = message .. " " .. v
	end
	
	Server.BroadcastMessage(message)
end