EssentialsPlugin.Emote_EmoteCommand = function(client, args)
	message = "&9* " .. client.name
	message = message .. " " .. table.concat(args, " ")
	Server.BroadcastMessage(message)
end
