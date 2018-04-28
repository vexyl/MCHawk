#ifndef WORLD_H_
#define WORLD_H_

#include "Map.hpp"
#include "Client.hpp"
#include "Position.hpp"

#include <string>
#include <vector>
#include <map>

class World {
public:
	World(std::string name);

	Map& GetMap() { return m_map; }
	bool GetActive() { return m_active; }
	Position GetSpawnPosition() { return m_spawnPosition; }
	std::string GetName() { return m_name; }

	void AddClient(Client* client);
	void RemoveClient(int8_t pid);

	void Load();
	bool SaveMapIfChanged();

	void SetActive(bool active);
	void SetSpawnPosition(Position spawnPosition);
	void SetOption(std::string option, std::string value);

	std::vector<std::string> GetOptionNames();
	std::string GetOption(std::string option);

	void Tick();

	void OnPosition(Client* client, struct cposp clientPos);
	void OnBlock(Client* client, struct cblockp clientBlock);
	void BroadcastMessage(std::string message);
	void SendBlockToClients(int type, short x, short y, short z);

private:
	enum { kAutosaveTime = 300 /* seconds */ };

	std::string m_name;
	Map m_map;
	Position m_spawnPosition;
	std::vector<Client*> m_clients;

	std::map<std::string, std::string> m_options;

	sf::Clock m_autosaveClock;

	bool m_active;
	bool m_mapChanged;
};

#endif // WORLD_H_
