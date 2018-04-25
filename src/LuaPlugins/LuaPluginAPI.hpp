#ifndef LUAPLUGINAPI_H_
#define LUAPLUGINAPI_H_


#include "../Server.hpp"

#include <boost/algorithm/string.hpp> // TEMPORARY

struct LuaServer {
	static void BroadcastMessage(std::string message)
	{
		Server::GetInstance()->BroadcastMessage(message);
	}

	static void KickClient(Client* client, std::string reason)
	{
		Server::GetInstance()->KickClient(client, reason);
	}
};

luabridge::LuaRef make_luatable(lua_State* L);
luabridge::LuaRef cauthp_to_luatable(lua_State* L, const struct cauthp clientAuth);
luabridge::LuaRef cmsgp_to_luatable(lua_State* L, const struct cmsgp clientMsg);

#endif // LUAPLUGINAPI_H_
