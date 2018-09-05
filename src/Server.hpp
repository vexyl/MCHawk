#ifndef SERVER_H_
#define SERVER_H_

#include <cstring>
#include <cassert>

#include <string>
#include <vector>
#include <map>
#include <functional>

#include <SFML/Network.hpp>
#include  <boost/signals2.hpp>

#include "Network/Protocol.hpp"
#include "Client.hpp"
#include "World.hpp"
#include "Position.hpp"
#include "CommandHandler.hpp"
#include "LuaPlugins/LuaPluginHandler.hpp"

class Server {
public:
	// FIXME: Temporary
	bool reloadPluginsFlag;

	Server();
	~Server();

	static Server* GetInstance()
	{
		if (m_thisPtr == nullptr) {
			m_thisPtr = new Server();
		}
		return m_thisPtr;
	}

	void FreeInstance();

	void Init();

	CommandHandler& GetCommandHandler() { return m_commandHandler; }
	LuaPluginHandler& GetPluginHandler() { return m_pluginHandler; }

	std::vector<Client*> GetClients() { return m_clients; }
	std::map<std::string, World*> GetWorlds() { return m_worlds; }
	std::string GetName() { return "&bMCHawk"; }

	void LoadPlugins();
	void ReloadPlugins();

	void OnConnect(sf::TcpSocket *sock);
	void OnAuth(Client* client, struct Protocol::cauthp clientAuth);
	void OnMessage(Client* client, struct Protocol::cmsgp clientMsg);

	void HandlePacket(Client* client, uint8_t opcode);
	bool Tick();

	void SendHeartbeat();

	// Client helper functions
	void KickClient(Client* client, std::string reason="");
	static void SendWrappedMessageB(Client* client, std::string message);
	static void SendWrappedMessage(Client* client, std::string message);
	void SendSystemMessage(Client* client, std::string message);
	void SendSystemWideMessage(std::string message);
	void BroadcastMessage(std::string message);

	Client* GetClientByName(std::string name, bool exact=true);

	bool IsOperator(std::string name);
	void Shutdown();

	void AddWorld(World* world);
	void RemoveWorld(std::string name);
	World* GetWorld(std::string name);
	World* GetWorldByName(std::string name, bool exact=false);

private:
	enum { kHeartbeatTime = 60 /* seconds */, kSaveTime = 600 /* seconds */ };

	static Server* m_thisPtr; // Singleton

	sf::TcpListener m_listener;

	unsigned short m_port;

	bool m_running;

	LuaPluginHandler m_pluginHandler;

	// Configuration
	std::string m_serverName;
	std::string m_serverMotd;
	std::string m_salt;
	int m_version;
	int m_numClients;
	int m_maxClients;
	bool m_serverHeartbeat;
	bool m_serverPublic;
	bool m_serverVerifyNames;

	std::vector<Client*> m_clients;

	CommandHandler m_commandHandler;

	sf::Clock m_heartbeatClock;

	std::map<std::string, World*> m_worlds;
};

#endif // SERVER_H_
