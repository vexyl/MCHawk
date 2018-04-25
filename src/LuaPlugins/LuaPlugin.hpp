#ifndef LUAPLUGIN_H_
#define LUAPLUGIN_H_

// https://eliasdaler.wordpress.com/2014/07/18/using-lua-with-cpp-luabridge/

#include "LuaStuff.hpp"

#include <memory>

class LuaPlugin {
public:
	void LoadScript(lua_State* L, const std::string& filename, const std::string& tableName);

	void Init();
	void Tick();

private:
	std::string m_name;
	std::unique_ptr<luabridge::LuaRef> m_initFunc;
	std::unique_ptr<luabridge::LuaRef> m_tickFunc;
};

#endif // PLUGIN_H_
