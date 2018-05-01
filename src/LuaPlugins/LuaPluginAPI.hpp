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
				.addFunction("GetWorld", &Client::GetWorld)
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<World>("World")
				.addFunction("Save", &World::Save)
				.addFunction("GetOption", &World::GetOption)
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<LuaServer>("Server")
				.addStaticFunction("SendMessage", &LuaServer::LuaSendMessage)
				.addStaticFunction("BroadcastMessage", &LuaServer::LuaBroadcastMessage)
				.addStaticFunction("SystemWideMessage", &LuaServer::LuaSystemWideMessage)
				.addStaticFunction("SendBlock", &LuaServer::LuaSendBlock)
				.addStaticFunction("KickClient", &LuaServer::LuaKickClient)
				.addStaticFunction("GetClientByName", &LuaServer::LuaGetClientByName)
				.addStaticFunction("LoadPlugin", &LuaServer::LuaLoadPlugin)
				.addStaticFunction("RegisterEvent", &LuaServer::LuaRegisterEvent)
				.addStaticFunction("AddCommand", &LuaServer::LuaAddCommand)
				.addStaticFunction("PlaceBlock", &LuaServer::LuaPlaceBlock)
				.addStaticFunction("MapGetBlockType", &LuaServer::LuaMapGetBlockType)
				.addStaticFunction("SendKick", &LuaServer::LuaSendKick)
				.addStaticFunction("GetClients", &LuaServer::LuaGetClients)
				.addStaticFunction("GetWorlds", &LuaServer::LuaGetWorlds)
				.addStaticFunction("Shutdown", &LuaServer::LuaServerShutdown)
			.endClass();
	}

	static void LuaSendMessage(Client* client, std::string message)
	{
		::SendMessage(client, message);
	}

	static void LuaSystemWideMessage(std::string message)
	{
		Server::GetInstance()->SendSystemWideMessage(message);
	}

	static void LuaBroadcastMessage(std::string message)
	{
		Server::GetInstance()->BroadcastMessage(message);
	}

	static void LuaKickClient(Client* client, std::string reason)
	{
		Server::GetInstance()->KickClient(client, reason);
	}

	static void LuaSendBlock(Client* client, short x, short y, short z, int type)
	{
		::SendBlock(client, Position(x, y, z), type);
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
			int type = LuaServer::LuaMapGetBlockType(client, x, y, z);
			SendBlock(client, Position(x, y, z), type);
			return;
		}

		world->SendBlockToClients(type, x, y, z);
	}

	static int LuaMapGetBlockType(Client* client, short x, short y, short z)
	{
		Map& map = Server::GetInstance()->GetWorld(client->GetWorldName())->GetMap();
		return map.GetBlockType(x, y, z);
	}

	static void LuaSendKick(Client* client, std::string reason)
	{
		::SendKick(client, reason);
	}

	static luabridge::LuaRef LuaGetClients()
	{
		std::vector<Client*> clients = Server::GetInstance()->GetClients();

		auto table = make_luatable();
		int i = 1;
		for (auto& obj : clients) {
			table[i] = obj;
			++i;
		}

		return table;
	}

	static luabridge::LuaRef LuaGetWorlds()
	{
		std::map<std::string, World*> worlds = Server::GetInstance()->GetWorlds();

		auto table = make_luatable();
		int i = 1;
		for (auto& obj : worlds) {
			table[i] = obj.second;
			++i;
		}

		return table;
	}

	static void LuaServerShutdown()
	{
		Server::GetInstance()->Shutdown();
	}
		
};

luabridge::LuaRef make_luatable();
luabridge::LuaRef cauthp_to_luatable(const struct cauthp clientAuth);
luabridge::LuaRef cmsgp_to_luatable(const struct cmsgp clientMsg);
luabridge::LuaRef cblockp_to_luatable(const struct cblockp clientBlock);

#endif // LUAPLUGINAPI_H_
