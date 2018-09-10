#ifndef LUAPLUGINHANDLER_H_
#define LUAPLUGINHANDLER_H_

#include "LuaPlugin.hpp"

#include "../Client.hpp"
#include "../Network/Protocol.hpp"

#include  <boost/signals2.hpp>
#include <iostream>

enum EventType { kOnConnect, kOnAuth, kOnMessage, kOnPosition, kOnBlock, kOnPluginLoaded, kOnDisconnect, kOnWorldJoin, kEventTypeEnd };

class LuaPluginHandler {
public:
  static lua_State* L;

  LuaPluginHandler();

  ~LuaPluginHandler();

  void Init();
  void Cleanup();
  void Reset();

  void AddPlugin(LuaPlugin* plugin);
  void LoadPlugin(std::string filename);
  void QueuePlugin(std::string filename);
  void FlushPluginQueue();
  void RegisterEvent(int type, luabridge::LuaRef func);
  void TriggerEvent(int type, Client* client, luabridge::LuaRef table);
  void TickPlugins();

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
      table[name] = 0;

    return flag;
  }

private:
  std::array<boost::signals2::signal<void(Client*, luabridge::LuaRef)>, kEventTypeEnd> m_signalMap;

  std::vector<LuaPlugin*> m_plugins;
  std::vector<std::string> m_pluginQueue;
};

#endif // LUAPLUGINHANDLER_H_
