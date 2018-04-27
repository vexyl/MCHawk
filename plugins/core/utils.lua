function mysplit(inputstr, sep)
if sep == nil then
        sep = "%s"
end
local t={} ; i=1
for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
        t[i] = str
        i = i + 1
end
return t
end

-- Do NOT use this to load plugins!
-- Only for functions/variables in the plugin's table
include = function(plugin, filename)
	dofile("plugins/" .. plugin.name .. "/" .. filename)
end
