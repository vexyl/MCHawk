#include "LuaPluginHandler.hpp"

#include "LuaPluginAPI.hpp"

#include <iostream>

std::array<boost::signals2::signal<void(Client*, luabridge::LuaRef)>, kEventTypeEnd> LuaPluginHandler::signalMap;

LuaPluginHandler::LuaPluginHandler()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	luabridge::getGlobalNamespace(L)	
		.addFunction("RegisterEvent", &LuaPluginHandler::RegisterEvent);

	luabridge::getGlobalNamespace(L)
		.beginClass<Client>("Client")
		.addProperty("name", &Client::GetName)
		.endClass();

	luabridge::getGlobalNamespace(L)
		.beginClass<LuaServer>("Server")
			.addStaticFunction("BroadcastMessage", &LuaServer::BroadcastMessage)
			.addStaticFunction("KickClient", &LuaServer::KickClient)
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
	signalMap[type](client, table);
}
