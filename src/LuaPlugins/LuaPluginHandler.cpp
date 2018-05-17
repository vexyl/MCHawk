#include "LuaPluginHandler.hpp"

#include "LuaPluginAPI.hpp"

#include <iostream>

lua_State* LuaPluginHandler::L = nullptr;

LuaPluginHandler::LuaPluginHandler()
{
	Init();
}

LuaPluginHandler::~LuaPluginHandler()
{
	Cleanup();
}

void LuaPluginHandler::Init()
{
	L = luaL_newstate();

	assert(L != nullptr);

	luaL_openlibs(L);

	LuaServer::Init(L);
}

void LuaPluginHandler::Cleanup()
{
	for (int i = 0; i < kEventTypeEnd; ++i) {
		m_signalMap[i].disconnect_all_slots();
		assert(m_signalMap[i].empty());
	}

	for (auto& obj : m_plugins)
		delete obj;

	lua_close(L);

	L = nullptr;
}

void LuaPluginHandler::Reset()
{
	Cleanup();

	m_plugins.clear();
	m_pluginQueue.clear();

	Init();
}
void LuaPluginHandler::AddPlugin(LuaPlugin* plugin)
{
	m_plugins.push_back(plugin);
}

void LuaPluginHandler::LoadPlugin(std::string filename)
{
	LuaPlugin* plugin = new LuaPlugin;

	if (!plugin->LoadScript(L, filename) || !plugin->Init()) {
		delete plugin;
		return;
	}

	AddPlugin(plugin);

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
			m_signalMap[type].connect(func);
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
