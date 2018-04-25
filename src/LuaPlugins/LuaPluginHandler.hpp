#ifndef LUAPLUGINHANDLER_H_
#define LUAPLUGINHANDLER_H_

#include "LuaPlugin.hpp"

#include "../Client.hpp"
#include "../Network/Protocol.hpp"

#include  <boost/signals2.hpp>

enum EventType { kOnConnect, kOnAuth, kOnMessage, kOnPosition, kOnBlock, kEventTypeEnd };

class LuaPluginHandler {
public:
	LuaPluginHandler();

	~LuaPluginHandler();

	static std::array<boost::signals2::signal<void(Client*, luabridge::LuaRef)>, kEventTypeEnd> signalMap;

	void AddPlugin(LuaPlugin* plugin);
	void LoadPlugin(std::string filename);
	static void RegisterEvent(int type, luabridge::LuaRef func);
	void TriggerEvent(int type, Client* client, luabridge::LuaRef table);

	lua_State* GetLuaState() { return L; }

private:
	lua_State* L;

	std::vector<LuaPlugin*> m_plugins;

	boost::signals2::signal<void(Client*, luabridge::LuaRef)>
		m_onConnectSignal, m_onAuthSignal, m_onMessageSignal,
		m_onPositionSignal, m_onBlockSignal;
};

#endif // LUAPLUGINHANDLER_H_
