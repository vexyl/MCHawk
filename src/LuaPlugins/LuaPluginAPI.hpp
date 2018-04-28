#ifndef LUAPLUGINAPI_H_
#define LUAPLUGINAPI_H_


#include "../Server.hpp"
#include "../Network/Protocol.hpp"
#include "LuaCommand.hpp"

void AddCommand(std::string name, std::string aliases, luabridge::LuaRef func, std::string docString,
			unsigned argumentAmount, unsigned permissionLevel);

void PlaceBlock(Client* client, uint8_t type, short x, short y, short z);

struct LuaServer {
	static void Init(lua_State* L)
	{
		luabridge::getGlobalNamespace(L)	
			.addFunction("AddCommand", &AddCommand)
			.addFunction("RegisterEvent", &LuaPluginHandler::RegisterEvent)
			.addFunction("PlaceBlock", &PlaceBlock);

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
				.addStaticFunction("LoadPlugin", &LuaServer::LuaLoadPlugin)
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

	static Client* LuaGetClientByName(std::string name, bool exact)
	{
		return Server::GetInstance()->GetClientByName(name, exact);
	}

	static void LuaLoadPlugin(std::string name)
	{
		Server::GetInstance()->GetPluginHandler().LoadPlugin(name);
	}
};

luabridge::LuaRef make_luatable();
luabridge::LuaRef cauthp_to_luatable(const struct cauthp clientAuth);
luabridge::LuaRef cmsgp_to_luatable(const struct cmsgp clientMsg);
luabridge::LuaRef cblockp_to_luatable(const struct cblockp clientBlock);

#endif // LUAPLUGINAPI_H_
