#ifndef WORLD_H_
#define WORLD_H_

#include "Map.hpp"
#include "Client.hpp"
#include "Position.hpp"
#include "Network/Protocol.hpp"

#include <string>
#include <vector>
#include <map>

class World {
public:
	World();
	World(std::string name);

	Map& GetMap() { return m_map; }
	bool GetActive() { return m_active; }
	Position GetSpawnPosition() { return m_spawnPosition; }
	std::string GetName() { return m_name; }

	void AddClient(Client* client);
	void RemoveClient(int8_t pid);

	void Load(std::string filename);
	void Save();

	void SetActive(bool active);
	void SetSpawnPosition(Position spawnPosition);
	bool SetOption(std::string option, std::string value, bool createNewOption=false);
	bool IsValidOption(std::string option);

	std::vector<std::string> GetOptionNames();
	std::string GetOption(std::string option);

	void Tick();

	void OnPosition(Client* client, struct Protocol::cposp clientPos);
	void OnBlock(Client* client, struct Protocol::cblockp clientBlock);
	void BroadcastMessage(std::string message);
	void SendBlockToClients(uint8_t type, short x, short y, short z);

private:
	enum { kAutosaveTime = 300 /* seconds */ };

	std::string m_name;
	Map m_map;
	Position m_spawnPosition;
	std::vector<Client*> m_clients;

	std::map<std::string, std::string> m_options;

	sf::Clock m_autosaveClock;

	bool m_active;
	bool m_saveFlag;
};

#endif // WORLD_H_
