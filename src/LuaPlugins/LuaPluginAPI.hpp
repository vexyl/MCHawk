#ifndef LUAPLUGINAPI_H_
#define LUAPLUGINAPI_H_


#include "../Server.hpp"
#include "../Network/Protocol.hpp"

struct LuaServer {
	static void LuaSendMessage(Client* client, std::string message)
	{
		::SendMessage(client, message);
	}

	static void LuaBroadcastMessage(std::string message)
	{
		Server::GetInstance()->BroadcastMessage(message);
	}

	static void LuaKickClient(Client* client, std::string reason)
	{
		Server::GetInstance()->KickClient(client, reason);
	}

	static Client* LuaGetClientByName(std::string name)
	{
		return Server::GetInstance()->GetClientByName(name);
	}
};

luabridge::LuaRef make_luatable();
luabridge::LuaRef cauthp_to_luatable(const struct cauthp clientAuth);
luabridge::LuaRef cmsgp_to_luatable(const struct cmsgp clientMsg);

#endif // LUAPLUGINAPI_H_
