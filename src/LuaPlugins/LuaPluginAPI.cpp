#include "LuaPluginAPI.hpp"

// Struct to table stuff
luabridge::LuaRef make_luatable(lua_State* L)
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	return table;
}

luabridge::LuaRef cauthp_to_luatable(lua_State* L, const struct cauthp clientAuth)
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	std::string name((char*)clientAuth.name, 0, sizeof(clientAuth.name));

	// Remove 0x20 padding
	boost::trim_right(name);

	table["name"] = name;

	return table;
}

luabridge::LuaRef cmsgp_to_luatable(lua_State* L, const struct cmsgp clientMsg)
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	std::string message((char*)clientMsg.msg, 0, sizeof(clientMsg.msg));

	// Remove 0x20 padding
	boost::trim_right(message);

	table["message"] = message;

	return table;
}
