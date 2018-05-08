split = function(str, delim)
	local t = {}

	if (delim == nil) then
		delim = "%s" -- whitespace
	end

	local i = 1
	for s in string.gmatch(str, "([^" .. delim .. "]+)") do
		t[i] = s
		i = i + 1
	end

	return t
end

-- Do NOT use this to load plugins!
-- Only for functions/variables in the plugin's table
include = function(plugin, filename)
	dofile("plugins/" .. plugin.name .. "/" .. filename)
end

LoadPlugin = function(plugin, filename)
	Server.LoadPlugin("plugins/" .. plugin.name .. "/" .. filename)
end

GetTableLength = function(t)
	local length = 0
	for _ in pairs(t) do
		length = length + 1
	end

	return length
end

-- Stolen from https://stackoverflow.com/questions/6118799/creating-a-timer-using-lua
timer = function(time, func)
	local init = os.time()
	local diff = os.difftime(os.time(), init)
	while diff < time do
		coroutine.yield(diff)
		diff = os.difftime(os.time(), init)
	end

	func()
end

PLAYER_NOT_FOUND = function(name)
	return "&cPlayer &f" .. name .. " &cnot found"
end

WORLD_NOT_FOUND = function(name)
	return "&cWorld &f" .. name .. " &cnot found"
end
