﻿#include "World.hpp"

#include "Client.hpp"
#include "Network/Protocol.hpp"
#include "Network/CPE.hpp"
#include "Utils/Logger.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

// m_mapChanged set to true so when the .ini first saves the map file gets saved too
World::World(std::string name) : m_name(name), m_active(false), m_mapChanged(true)
{
	SetOption("build", "true");
	SetOption("autosave", "false");
	SetOption("autoload", "false");
}

World::World() : World("")
{

}

void World::Load(std::string filename)
{
	try {
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini(filename, pt);

		std::string name = pt.get<std::string>("World.name");
		std::string mapFilename = pt.get<std::string>("World.map");

		short x_size = pt.get<short>("Size.x");
		short y_size = pt.get<short>("Size.y");
		short z_size = pt.get<short>("Size.z");

		short sx = pt.get<short>("Spawn.x");
		short sy = pt.get<short>("Spawn.y");
		short sz = pt.get<short>("Spawn.z");

		std::string autosave = pt.get<std::string>("Options.autosave");
		std::string build = pt.get<std::string>("Options.build");
		std::string autoload = pt.get<std::string>("Options.autoload");

		m_name = name;
		m_map.SetDimensions(x_size, y_size, z_size);
		m_map.SetFilename("worlds/" + mapFilename);
		SetSpawnPosition(Position(sx, sy, sz));
		SetOption("autosave", autosave);
		SetOption("build", build);
		SetOption("autoload", autoload);

		if (autoload == "true") {
			m_map.Load();
			SetActive(true);
		}
	} catch (std::runtime_error& e) {
		LOG(LogLevel::kWarning, "%s", e.what());
	}
}

void World::Save()
{
	try {
		boost::property_tree::ptree pt;

		std::string name = m_name;
		std::string filename = m_map.GetFilename();

		std::size_t pos = filename.find("worlds/");
		if (pos != std::string::npos)
			filename = filename.substr(pos + strlen("worlds/"));

		short x_size = m_map.GetXSize();
		short y_size = m_map.GetYSize();
		short z_size = m_map.GetZSize();

		short x_spawn = m_spawnPosition.x;
		short y_spawn = m_spawnPosition.y;
		short z_spawn = m_spawnPosition.z;

		std::string autoload = GetOption("autoload");
		std::string autosave = GetOption("autosave");
		std::string build = GetOption("build");

		pt.add("World.name", name);
		pt.add("World.map", filename);

		pt.add("Size.x", x_size);
		pt.add("Size.y", y_size);
		pt.add("Size.z", z_size);

		pt.add("Spawn.x", x_spawn);
		pt.add("Spawn.y", y_spawn);
		pt.add("Spawn.z", z_spawn);

		pt.add("Options.autoload", autoload);
		pt.add("Options.autosave", autosave);
		pt.add("Options.build", build);

		boost::property_tree::ini_parser::write_ini("worlds/" + m_name + ".ini", pt);
	} catch (std::runtime_error& e) {
		LOG(LogLevel::kWarning, "%s", e.what());
	}

	if (GetActive())
		SaveMapIfChanged();
}

void World::AddClient(Client* client)
{
	LOG(LogLevel::kDebug, "Player with pid=%d added to world '%s'", client->GetPid(), m_name.c_str());

	Protocol::SendMap(client, m_map);
	Protocol::SpawnClient(client, m_spawnPosition, m_clients);
	Protocol::SendClientsTo(client, m_clients);

	client->SetWorld(this);

	m_clients.push_back(client);
}

void World::RemoveClient(int8_t pid)
{
	auto iter = m_clients.begin();
	while (iter != m_clients.end()) {
		if ((*iter)->GetPid() == pid) {
			m_clients.erase(iter);
			LOG(LogLevel::kDebug, "Player with pid=%d removed from world '%s'", pid, m_name.c_str());
			Protocol::DespawnClient(pid, m_clients);
			break;
		}

		++iter;
	}
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

void World::SetActive(bool active)
{
	m_active = active;
}

void World::SetSpawnPosition(Position spawnPosition)
{
	m_spawnPosition = spawnPosition;
}

void World::SetOption(std::string option, std::string value)
{
	m_options[option] = value;
}

std::vector<std::string> World::GetOptionNames()
{
	std::vector<std::string> options;

	for (auto& option : m_options)
		options.push_back(option.first);

	return options;
}

std::string World::GetOption(std::string option)
{
	return m_options[option];
}

bool World::IsValidOption(std::string option)
{
	bool result = false;

	for (auto& obj : m_options) {
		if (option == obj.first)
			result = true;
	}

	return result;
}

void World::Tick()
{
	if (GetOption("autosave") == "true") {
		if (m_autosaveClock.getElapsedTime().asSeconds() >= kAutosaveTime) {
			if (SaveMapIfChanged())
				BroadcastMessage("Map saved");
			Save(); // Saves settings, skips map save since SaveMapIfChanged() was called above
			m_autosaveClock.restart();
		}
	}
}

void World::OnPosition(Client* client, struct Protocol::cposp clientPos)
{
	client->SetPositionOrientation(Position(clientPos.x, clientPos.y, clientPos.z), clientPos.yaw, clientPos.pitch);
	Protocol::SendPlayerPositionUpdate(client, m_clients);
}

void World::OnBlock(Client* client, struct Protocol::cblockp clientBlock)
{
	uint8_t type = clientBlock.type;
	// If player is breaking a block, set block type to air
	if (clientBlock.mode == 0x00) // Breaking mode
		type = 0x00; // Air block type

	Position position(clientBlock.x, clientBlock.y, clientBlock.z);

	if (GetOption("build") == "false") {
		type = m_map.GetBlockType(clientBlock.x, clientBlock.y, clientBlock.z);
		Protocol::SendBlock(client, position, type);
		Protocol::SendMessage(client, "&cNo building here");
		return;
	}

	bool validBlock = Protocol::IsValidBlock(type) || CPE::IsValidBlock(type);
	if (!validBlock) {
		Protocol::SendBlock(client, position, 0x00); // Invalid block, tell client to destroy it
		Protocol::SendMessage(client, "&cInvalid block type");
		return;
	}

	bool validPlace = m_map.SetBlock(clientBlock.x, clientBlock.y, clientBlock.z, type);
	if (!validPlace) {
		Protocol::SendBlock(client, position, 0x00); // Invalid block, tell client to destroy it
		return;
	}

	uint8_t pid = client->GetPid();
	// Broadcast block changes to all other clients
	for (auto& obj : m_clients) {
		if (obj->GetPid() != pid) {
			Protocol::SendBlock(obj, position, type);
		}
	}

	m_mapChanged = true;
}

// FIXME: Send wrapped message
void World::BroadcastMessage(std::string message)
{
	for (auto& obj : m_clients)
		Protocol::SendMessage(obj, "&e[WORLD]: " + message);
}

void World::SendBlockToClients(int type, short x, short y, short z)
{
	for (auto& obj : m_clients)
		Protocol::SendBlock(obj, Position(x, y, z), type);
}
