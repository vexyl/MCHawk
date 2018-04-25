// https://eliasdaler.wordpress.com/2014/07/18/using-lua-with-cpp-luabridge/

#include "LuaPlugin.hpp"

#include <iostream>

void LuaPlugin::LoadScript(lua_State* L, const std::string& filename, const std::string& tableName)
{
	if (luaL_dofile(L, filename.c_str()) == 0) {
		luabridge::LuaRef table = luabridge::getGlobal(L, tableName.c_str());

		if (table.isTable()) {
			if (table["name"].isString())
				m_name = table["name"].cast<std::string>();
			else
				m_name = "Undefined";

			if (table["init"].isFunction()) {
				m_initFunc = std::make_unique<luabridge::LuaRef>(table["init"]);
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
		std::cerr << "Failed to load script " << filename << std::endl;
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
