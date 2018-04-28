#include "LuaPluginAPI.hpp"

#include <boost/algorithm/string.hpp>

extern lua_State* L;

void AddCommand(std::string name, std::string aliases, luabridge::LuaRef func, std::string docString,
			unsigned argumentAmount, unsigned permissionLevel)
{
	if (!func.isFunction()) {
		std::cerr << "Failed adding Lua command " << name << ": function does not exist" << std::endl;
		return;
	}

	LuaCommand* command = new LuaCommand(name, func, docString, argumentAmount, permissionLevel);

	Server::GetInstance()->GetCommandHandler().Register(name, command, aliases);
}

void PlaceBlock(Client* client, uint8_t type, short x, short y, short z)
{
	World* world = Server::GetInstance()->GetWorld(client->GetWorldName());

	bool valid = world->GetMap().SetBlock(x, y, z, type);

	if (!valid) {
		// TODO: Send block back
		return;
	}

	world->SendBlockToClients(type, x, y, z);
}

// Struct to table stuff
luabridge::LuaRef make_luatable()
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	return table;
}

luabridge::LuaRef cauthp_to_luatable(const struct cauthp clientAuth)
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	std::string name((char*)clientAuth.name, 0, sizeof(clientAuth.name));

	// Remove 0x20 padding
	boost::trim_right(name);

	table["name"] = name;

	return table;
}

luabridge::LuaRef cmsgp_to_luatable(const struct cmsgp clientMsg)
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	std::string message((char*)clientMsg.msg, 0, sizeof(clientMsg.msg));

	// Remove 0x20 padding
	boost::trim_right(message);

	table["message"] = message;

	return table;
}

luabridge::LuaRef cblockp_to_luatable(const struct cblockp clientBlock)
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	table["type"] = clientBlock.type;
	table["mode"] = clientBlock.mode;
	table["x"] = clientBlock.x;
	table["y"] = clientBlock.y;
	table["z"] = clientBlock.z;

	return table;
}
