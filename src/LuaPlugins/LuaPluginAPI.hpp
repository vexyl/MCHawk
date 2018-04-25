#ifndef LUAPLUGINAPI_H_
#define LUAPLUGINAPI_H_


#include "../Server.hpp"
#include "../Network/Protocol.hpp"
#include "LuaCommand.hpp"

void AddCommand(std::string name, luabridge::LuaRef func, std::string docString,
			unsigned argumentAmount, unsigned permissionLevel);

struct LuaServer {
	static void Init(lua_State* L)
	{
		luabridge::getGlobalNamespace(L)	
			.addFunction("AddCommand", &AddCommand)
			.addFunction("RegisterEvent", &LuaPluginHandler::RegisterEvent);

		luabridge::getGlobalNamespace(L)
			.beginClass<Client>("Client")
				.addProperty("name", &Client::GetName)
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<LuaServer>("Server")
				.addStaticFunction("SendMessage", &LuaServer::LuaSendMessage)
				.addStaticFunction("BroadcastMessage", &LuaServer::LuaBroadcastMessage)
				.addStaticFunction("KickClient", &LuaServer::LuaKickClient)
				.addStaticFunction("GetClientByName", &LuaServer::LuaGetClientByName)
			.endClass();
	}

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
