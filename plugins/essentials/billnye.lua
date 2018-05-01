-- Yes, this is an essential command
EssentialsPlugin.BillNye_BillNyeCommand = function(client, args)
	message = table.concat(args, " ")

	Server.BroadcastMessage("&cBill Nye&f: DID YOU KNOW THAT")
	Server.BroadcastMessage("&cBill Nye&f: " .. message .. "?")
	Server.BroadcastMessage("&cBill Nye&f: NOW YOU KNOW!")
end
