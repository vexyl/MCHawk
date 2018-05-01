#include "LuaPluginHandler.hpp"

#include "LuaPluginAPI.hpp"

#include <iostream>

lua_State* L;

LuaPluginHandler::LuaPluginHandler()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	LuaServer::Init(L);
}

LuaPluginHandler::~LuaPluginHandler()
{
	for (auto& obj : m_plugins)
		delete obj;
}

void LuaPluginHandler::AddPlugin(LuaPlugin* plugin)
{
	m_plugins.push_back(plugin);
}

void LuaPluginHandler::LoadPlugin(std::string filename)
{
	LuaPlugin* plugin = new LuaPlugin;

	plugin->LoadScript(L, filename);
	AddPlugin(plugin);
	plugin->Init();

	auto table = make_luatable();

	table["name"] = plugin->GetName();

	TriggerEvent(EventType::kOnPluginLoaded, nullptr, table);
}

void LuaPluginHandler::QueuePlugin(std::string filename)
{
	m_pluginQueue.push_back(filename);
}

void LuaPluginHandler::FlushPluginQueue()
{
	for (auto& filename : m_pluginQueue)
		LoadPlugin(filename);

	m_pluginQueue.clear();
}

void LuaPluginHandler::RegisterEvent(int type, luabridge::LuaRef func)
{
	if (func.isFunction()) {
		try {
			m_signalMap[type].connect(boost::bind((std::function<void(Client*, luabridge::LuaRef)>)func, _1, _2));
		} catch (luabridge::LuaException const& e) {
			std::cerr << "LuaException: " << e.what() << std::endl;
		}
	}
}

void LuaPluginHandler::TriggerEvent(int type, Client* client, luabridge::LuaRef table)
{
	try {
		m_signalMap[type](client, table);
	} catch (luabridge::LuaException const& e) {
		std::cerr << "LuaException: " << e.what() << std::endl;
	}
}

void LuaPluginHandler::TickPlugins()
{
	for (auto& obj : m_plugins)
		obj->Tick();
}
