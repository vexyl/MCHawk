#include "LuaPluginHandler.hpp"

#include "LuaPluginAPI.hpp"
#include "LuaCommand.hpp"

#include <iostream>

std::array<boost::signals2::signal<void(Client*, luabridge::LuaRef)>, kEventTypeEnd> LuaPluginHandler::signalMap;

lua_State* L;

void AddCommand(std::string name, luabridge::LuaRef func, std::string docString,
			unsigned argumentAmount, unsigned permissionLevel)
{
	if (!func.isFunction()) {
		std::cerr << "Failed adding Lua command " << name << ": function does not exist" << std::endl;
		return;
	}

	LuaCommand* command = new LuaCommand(name, func, docString, argumentAmount, permissionLevel);

	Server::GetInstance()->GetCommandHandler().Register(name, command);
}

LuaPluginHandler::LuaPluginHandler()
{
	L = luaL_newstate();
	luaL_openlibs(L);

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

	if (luaL_dofile(L, "plugins/init.lua") != 0)
		std::cerr << "Failed to load init.lua" << std::endl;
}

LuaPluginHandler::~LuaPluginHandler()
{
	for (auto& obj : m_plugins)
		delete obj;
}

void LuaPluginHandler::AddPlugin(LuaPlugin* plugin)
{
	m_plugins.push_back(plugin);
	plugin->Init();
}

void LuaPluginHandler::LoadPlugin(std::string filename)
{
	LuaPlugin* plugin = new LuaPlugin;
	plugin->LoadScript(L, filename, "Plugin");
	AddPlugin(plugin);
}

void LuaPluginHandler::RegisterEvent(int type, luabridge::LuaRef func)
{
	std::cout << "called RegisterEvent type=" << type << std::endl;
	if (func.isFunction()) {
		try {
			signalMap[type].connect(boost::bind((std::function<void(Client*, luabridge::LuaRef)>)func, _1, _2));
		} catch (luabridge::LuaException const& e) {
			std::cerr << "LuaException: " << e.what() << std::endl;
		}
	}
}

void LuaPluginHandler::TriggerEvent(int type, Client* client, luabridge::LuaRef table)
{
	try {
		signalMap[type](client, table);
	} catch (luabridge::LuaException const& e) {
		std::cerr << "LuaException: " << e.what() << std::endl;
	}
}
