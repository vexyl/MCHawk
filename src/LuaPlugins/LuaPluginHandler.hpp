#ifndef LUAPLUGINHANDLER_H_
#define LUAPLUGINHANDLER_H_

#include "LuaPlugin.hpp"

#include "../Client.hpp"
#include "../Network/Protocol.hpp"

#include  <boost/signals2.hpp>
#include <iostream>

enum EventType { kOnConnect, kOnAuth, kOnMessage, kOnPosition, kOnBlock, kEventTypeEnd };

class LuaPluginHandler {
public:
	LuaPluginHandler();

	~LuaPluginHandler();

	static std::array<boost::signals2::signal<void (Client*, luabridge::LuaRef)>, kEventTypeEnd> signalMap;

	void AddPlugin(LuaPlugin* plugin);
	void LoadPlugin(std::string filename);
	static void RegisterEvent(int type, luabridge::LuaRef func);
	void TriggerEvent(int type, Client* client, luabridge::LuaRef table);

	lua_State* GetLuaState() { return L; }

	int GetEventFlag(std::string name)
	{
		auto table = luabridge::getGlobal(L, "Flags");

		if (!table.isTable())
			return -1;

		auto t = table[name];

		if (t.isNil())
			return -1;

		int flag = t.cast<int>();

		if (flag)
			table[name] = false;

		std::cout << "flag=" << flag << std::endl;
		return flag;
	}

private:
	lua_State* L;

	std::vector<LuaPlugin*> m_plugins;
};

#endif // LUAPLUGINHANDLER_H_
