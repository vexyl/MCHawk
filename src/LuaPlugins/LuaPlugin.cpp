// https://eliasdaler.wordpress.com/2014/07/18/using-lua-with-cpp-luabridge/

#include "LuaPlugin.hpp"

#include <iostream>

// TODO: Return value or throw exception and only Init() if no errors
void LuaPlugin::LoadScript(lua_State* L, const std::string& filename)
{
	if (luaL_dofile(L, filename.c_str()) == 0) {
		luabridge::LuaRef thisString = luabridge::getGlobal(L, "this");

		// TODO: check if name is string

		std::string tableName = thisString.cast<std::string>();

		luabridge::LuaRef pluginTable = luabridge::getGlobal(L, tableName.c_str());

		if (pluginTable.isTable()) {
			luabridge::LuaRef pluginName = pluginTable["name"];

			m_name = pluginName.cast<std::string>();

			if (pluginTable["init"].isFunction()) {
				m_initFunc = std::make_unique<luabridge::LuaRef>(pluginTable["init"]);
			} else {
				std::cerr << "Couldn't find init function for plugin " << m_name << " (" << filename << ")" << std::endl;
				return;
			}
		} else {
			std::cerr << "Couldn't find Plugin table for plugin " << m_name << " (" << filename << ")" << std::endl;
			return;
		}

		std::cout << "Loaded script " << filename << std::endl;
	} else {
		std::cerr << "Failed to load script " << filename << ": " << lua_tostring(L, -1) << std::endl;
	}
}

void LuaPlugin::Init()
{
	if (m_initFunc != nullptr) {
		try {
			(*m_initFunc)();
		} catch (luabridge::LuaException const& e) {
			std::cerr << "LuaException: " << e.what() << std::endl;
		}
	}
}

void LuaPlugin::Tick()
{

}
