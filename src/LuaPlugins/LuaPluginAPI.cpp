#include "LuaPluginAPI.hpp"

#include <boost/algorithm/string.hpp>

extern lua_State* L;

void LuaServer::Init(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginClass<Client>("Client")
		.addProperty("name", &Client::GetName)
		.addFunction("GetWorld", &Client::GetWorld)
		.addFunction("GetIpString", &Client::GetIpString)
	.endClass();

	luabridge::getGlobalNamespace(L)
	.beginClass<World>("World")
		.addFunction("Save", &World::Save)
		.addFunction("GetOption", &World::GetOption)
	.endClass();

	luabridge::getGlobalNamespace(L)
	.beginClass<LuaCommand>("LuaCommand")
		.addFunction("AddSubcommand", &LuaCommand::AddSubcommand)
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
		.addStaticFunction("GetCommandStrings", &LuaServer::LuaGetCommandStrings)
	.endClass();
}

void LuaServer::LuaSendMessage(Client* client, std::string message)
{
	Server::GetInstance()->SendWrappedMessage(client, message);
}

void LuaServer::LuaSystemWideMessage(std::string message)
{
	Server::GetInstance()->SendSystemWideMessage(message);
}

void LuaServer::LuaBroadcastMessage(std::string message)
{
	Server::GetInstance()->BroadcastMessage(message);
}

void LuaServer::LuaKickClient(Client* client, std::string reason)
{
	Server::GetInstance()->KickClient(client, reason);
}

void LuaServer::LuaSendBlock(Client* client, short x, short y, short z, int type)
{
	Protocol::SendBlock(client, Position(x, y, z), type);
}

Client* LuaServer::LuaGetClientByName(std::string name, bool exact)
{
	return Server::GetInstance()->GetClientByName(name, exact);
}

void LuaServer::LuaLoadPlugin(std::string filename)
{
	Server::GetInstance()->GetPluginHandler().QueuePlugin(filename);
}

void LuaServer::LuaRegisterEvent(int type, luabridge::LuaRef func)
{
	Server::GetInstance()->GetPluginHandler().RegisterEvent(type, func);
}

LuaCommand* LuaServer::LuaAddCommand(std::string name, std::string aliases, luabridge::LuaRef func, std::string docString,
	unsigned argumentAmount, unsigned permissionLevel)
{
	if (!func.isFunction()) {
		std::cerr << "Failed adding Lua command " << name << ": function does not exist" << std::endl;
		return nullptr;
	}

	LuaCommand* command = new LuaCommand(name, func, docString, argumentAmount, permissionLevel);

	Server::GetInstance()->GetCommandHandler().Register(name, command, aliases);

	return command;
}

void LuaServer::LuaPlaceBlock(Client* client, int type, short x, short y, short z)
{
	World* world = client->GetWorld();
	Map& map = world->GetMap();

	bool valid = map.SetBlock(x, y, z, type);

	if (!valid) {
		int type = map.GetBlockType(x, y, z);
		Protocol::SendBlock(client, Position(x, y, z), type);
		return;
	}

	world->SendBlockToClients(type, x, y, z);
}

int LuaServer::LuaMapGetBlockType(Client* client, short x, short y, short z)
{
	return client->GetWorld()->GetMap().GetBlockType(x, y, z);
}

void LuaServer::LuaSendKick(Client* client, std::string reason)
{
	Protocol::SendKick(client, reason);
}

luabridge::LuaRef LuaServer::LuaGetClients()
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

luabridge::LuaRef LuaServer::LuaGetWorlds()
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

void LuaServer::LuaServerShutdown()
{
	Server::GetInstance()->Shutdown();
}

luabridge::LuaRef LuaServer::LuaGetCommandStrings()
{
	CommandMap commands = Server::GetInstance()->GetCommandHandler().GetCommandList();

	auto table = make_luatable();
	int i = 1;
	for (auto& obj : commands) {
		table[i] = obj.first;
		++i;
	}

	return table;
}

// Struct to table stuff
luabridge::LuaRef make_luatable()
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	return table;
}

luabridge::LuaRef cauthp_to_luatable(const struct Protocol::cauthp clientAuth)
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	std::string name((char*)clientAuth.name, 0, sizeof(clientAuth.name));

	// Remove 0x20 padding
	boost::trim_right(name);

	table["name"] = name;

	return table;
}

luabridge::LuaRef cmsgp_to_luatable(const struct Protocol::cmsgp clientMsg)
{
	luabridge::LuaRef table(L);
	table = luabridge::newTable(L);

	std::string message((char*)clientMsg.msg, 0, sizeof(clientMsg.msg));

	// Remove 0x20 padding
	boost::trim_right(message);

	table["message"] = message;

	return table;
}

luabridge::LuaRef cblockp_to_luatable(const struct Protocol::cblockp clientBlock)
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
