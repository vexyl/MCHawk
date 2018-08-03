#ifndef LUAPLUGINAPI_H_
#define LUAPLUGINAPI_H_

#include "../Server.hpp"
#include "../Network/Protocol.hpp"
#include "LuaCommand.hpp"

struct LuaServer {
	static void Init(lua_State* L);

	static void LuaSendMessage(Client* client, std::string message);
	static void LuaSystemWideMessage(std::string message);
	static void LuaBroadcastMessage(std::string message);
	static void LuaKickClient(Client* client, std::string reason);
	static void LuaSendBlock(Client* client, short x, short y, short z, int type);
	static Client* LuaGetClientByName(std::string name, bool exact);
	static void LuaLoadPlugin(std::string filename);
	static void LuaRegisterEvent(int type, luabridge::LuaRef func);
	static LuaCommand* LuaAddCommand(std::string name, std::string aliases, luabridge::LuaRef func, std::string docString,
		unsigned argumentAmount, unsigned permissionLevel);
	static void LuaPlaceBlock(Client* client, int type, short x, short y, short z);
	static int LuaMapGetBlockType(Client* client, short x, short y, short z);
	static void LuaSendKick(Client* client, std::string reason);
	static luabridge::LuaRef LuaGetClients();
	static luabridge::LuaRef LuaGetWorlds();
	static World* LuaGetWorldByName(std::string name, bool exact);
	static void LuaServerShutdown();
	static luabridge::LuaRef LuaGetCommandStrings();
	static bool LuaIsOperator(std::string name);
	static void LuaTransportPlayer(Client* client, World* world);
	static luabridge::LuaRef LuaWorldGetOptionNames(World* world);
	static void LuaReloadPlugins();
	static void LuaCreateWorld(std::string worldName, short x, short y, short z);
	static void LuaLogError(std::string message);
	static void LuaLogWarning(std::string message);
	static void LuaLogInfo(std::string message);
	static void LuaLogDebug(std::string message);
	static void LuaLog(std::string message);
};

luabridge::LuaRef make_luatable();
luabridge::LuaRef cauthp_to_luatable(const struct Protocol::cauthp clientAuth);
luabridge::LuaRef cmsgp_to_luatable(const struct Protocol::cmsgp clientMsg);
luabridge::LuaRef cblockp_to_luatable(const struct Protocol::cblockp clientBlock);

#endif // LUAPLUGINAPI_H_
