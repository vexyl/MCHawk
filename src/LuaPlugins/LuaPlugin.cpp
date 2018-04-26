// https://eliasdaler.wordpress.com/2014/07/18/using-lua-with-cpp-luabridge/

#include "LuaPlugin.hpp"

#include <iostream>

// TODO: Return value or throw exception and only Init() if no errors
void LuaPlugin::LoadScript(lua_State* L, const std::string& filename)
{
	if (luaL_dofile(L, filename.c_str()) == 0) {
		luabridge::LuaRef name = luabridge::getGlobal(L, "this");

		// TODO: check if name is string

		m_name = name.cast<std::string>();
std::cout << "m_name=" << m_name << std::endl;
		luabridge::LuaRef table = luabridge::getGlobal(L, m_name.c_str());

		if (table.isTable()) {
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
