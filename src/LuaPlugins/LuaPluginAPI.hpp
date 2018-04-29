#ifndef LUAPLUGINAPI_H_
#define LUAPLUGINAPI_H_


#include "../Server.hpp"
#include "../Network/Protocol.hpp"
#include "LuaCommand.hpp"

struct LuaServer {
	static void Init(lua_State* L)
	{
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
				.addStaticFunction("RegisterEvent", &LuaServer::LuaRegisterEvent)
				.addStaticFunction("AddCommand", &LuaServer::LuaAddCommand)
				.addStaticFunction("PlaceBlock", &LuaServer::LuaPlaceBlock)
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

	static void LuaLoadPlugin(std::string filename)
	{
		Server::GetInstance()->GetPluginHandler().QueuePlugin(filename);
	}

	static void LuaRegisterEvent(int type, luabridge::LuaRef func)
	{
		Server::GetInstance()->GetPluginHandler().RegisterEvent(type, func);
	}

	static void LuaAddCommand(std::string name, std::string aliases, luabridge::LuaRef func, std::string docString,
			unsigned argumentAmount, unsigned permissionLevel)
	{
		if (!func.isFunction()) {
			std::cerr << "Failed adding Lua command " << name << ": function does not exist" << std::endl;
			return;
		}

		LuaCommand* command = new LuaCommand(name, func, docString, argumentAmount, permissionLevel);

		Server::GetInstance()->GetCommandHandler().Register(name, command, aliases);
	}

	static void LuaPlaceBlock(Client* client, int type, short x, short y, short z)
	{
		World* world = Server::GetInstance()->GetWorld(client->GetWorldName());

		bool valid = world->GetMap().SetBlock(x, y, z, type);

		if (!valid) {
			// TODO: Send block back
			return;
		}

		world->SendBlockToClients(type, x, y, z);
	}
};

luabridge::LuaRef make_luatable();
luabridge::LuaRef cauthp_to_luatable(const struct cauthp clientAuth);
luabridge::LuaRef cmsgp_to_luatable(const struct cmsgp clientMsg);
luabridge::LuaRef cblockp_to_luatable(const struct cblockp clientBlock);

#endif // LUAPLUGINAPI_H_
