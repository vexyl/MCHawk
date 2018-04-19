#include "World.hpp"

#include "Client.hpp"
#include "Network/Protocol.hpp"
#include "Utils/Logger.hpp"

World::World(std::string name) : m_name(name), m_mapChanged(true)
{
	SetOption("build", "true");
	SetOption("autosave", "true");
}

void World::AddClient(Client* client)
{
	LOG(DEBUG, "Player with pid=%d added to world '%s'", client->GetPid(), m_name.c_str());

	SendMap(client, m_map);
	SpawnClient(client, m_spawnPosition, m_clients);
	SendClientsTo(client, m_clients);

	client->SetWorldName(m_name);

	m_clients.push_back(client);
}

void World::RemoveClient(int8_t pid)
{
	auto iter = m_clients.begin();
	while (iter != m_clients.end()) {
		if ((*iter)->GetPid() == pid) {
			m_clients.erase(iter);
			LOG(DEBUG, "Player with pid=%d removed from world '%s'", pid, m_name.c_str());
			DespawnClient(pid, m_clients);
			break;
		}

		++iter;
	}
}

void World::LoadMap(std::string filename, short x, short y, short z)
{
	m_map.LoadFromFile(filename);
	m_map.SetDimensions(x, y, z);
	m_mapChanged = false;
}

bool World::SaveMapIfChanged()
{
	if (m_mapChanged) {
		m_map.SaveToFile();
		m_mapChanged = false;
		return true;
	}

	return false;
}

void World::SetSpawnPosition(Position spawnPosition)
{
	m_spawnPosition = spawnPosition;
}

void World::SetOption(std::string option, std::string value)
{
	m_options[option] = value;
}

std::string World::GetOption(std::string option)
{
	return m_options[option];
}

void World::Tick()
{
	if (GetOption("autosave") == "true") {
		if (m_autosaveClock.getElapsedTime().asSeconds() >= kAutosaveTime) {
			if (SaveMapIfChanged())
				BroadcastMessage("Map saved");
			m_autosaveClock.restart();
		}
	}
}

void World::OnPosition(Client* client, struct cposp clientPos)
{
	client->SetPositionOrientation(Position(clientPos.x, clientPos.y, clientPos.z), clientPos.yaw, clientPos.pitch);
	SendPlayerPositionUpdate(client, m_clients);
}

void World::OnBlock(Client* client, struct cblockp clientBlock)
{
	uint8_t type = clientBlock.type;
	// If player is breaking a block, set block type to air
	if (clientBlock.mode == 0x00) // Breaking mode
		type = 0x00; // Air block type

/*
	Position cpos = client->GetPosition();
	if (Utils::DistanceCheck(kMaxSetBlockDistance, cpos.x/32, cpos.y/32, cpos.z/32, clientBlock.x, clientBlock.y, clientBlock.z))
		KickClient(client, "Cheat detected");
*/

	Position position(clientBlock.x, clientBlock.y, clientBlock.z);

	if (GetOption("build") == "false") {
		type = m_map.GetBlockType(clientBlock.x, clientBlock.y, clientBlock.z);
		SendBlock(client, position, type);
		SendMessage(client, "&cNo building here");
		return;
	}

	bool validPlace = m_map.SetBlock(clientBlock.x, clientBlock.y, clientBlock.z, type);

	if (!validPlace) {
		SendBlock(client, position, 0x00); // Invalid block, tell client to destroy it
	} else {
		uint8_t pid = client->GetPid();
		// Broadcast block changes to all other clients
		for (auto& obj : m_clients) {
			if (obj->GetPid() != pid) {
				SendBlock(obj, position, type);
			}
		}

		m_mapChanged = true;
	}
}

// FIXME: Send wrapped message
void World::BroadcastMessage(std::string message)
{
	for (auto& obj : m_clients)
		SendMessage(obj, "&e[WORLD]: " + message);
}
