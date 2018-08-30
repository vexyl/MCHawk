#include "World.hpp"

#include "Client.hpp"
#include "Network/Protocol.hpp"
#include "Network/CPE.hpp"
#include "Utils/Logger.hpp"
#include "LuaPlugins/LuaPluginAPI.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

// m_saveFlag set to true for new worlds so they'll be saved when autosave is set to true
World::World(std::string name) : m_name(name), m_active(false), m_saveFlag(true)
{
	SetOption("build", "true", true);
	SetOption("autosave", "false", true);
	SetOption("autoload", "false", true);
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

	m_saveFlag = false;
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

	if (m_saveFlag) {
		m_map.SaveToFile();
		m_saveFlag = false;
	}
}

void World::AddClient(Client* client)
{
	auto table = make_luatable(); // FIXME: Temporary, don't need this since we already have the strings

	table["world"] = this;
	table["prev"] = client->GetWorld();

	Server::GetInstance()->GetPluginHandler().TriggerEvent(EventType::kOnWorldJoin, client, table);

	LOG(LogLevel::kDebug, "Player %s added to world '%s'", client->GetName().c_str(), m_name.c_str());

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
			LOG(LogLevel::kDebug, "Player %s removed from world '%s'", (*iter)->GetName().c_str(), m_name.c_str());
			Protocol::DespawnClient(pid, m_clients);
			break;
		}

		++iter;
	}
}

void World::SetActive(bool active)
{
	m_active = active;
}

void World::SetSpawnPosition(Position spawnPosition)
{
	m_spawnPosition = spawnPosition;
}

bool World::SetOption(std::string option, std::string value, bool createNewOption)
{
	if (!IsValidOption(option) && !createNewOption)
		return false;
	m_options[option] = value;
	return true;
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
	if (!IsValidOption(option))
		return "";
	return m_options[option];
}

bool World::IsValidOption(std::string option)
{
	return m_options.find(option) != m_options.end();
}

void World::Tick()
{
	if (!m_active)
		return;

	if (GetOption("autosave") == "true") {
		if (m_autosaveClock.getElapsedTime().asSeconds() >= kAutosaveTime) {
			Save();
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

	bool validBlock = Protocol::IsValidBlock(type) || CPE::IsValidBlock(type);
	if (!validBlock) {
		Protocol::SendBlock(client, position, 0x00); // Invalid block, tell client to destroy it
		Protocol::SendMessage(client, "&cInvalid block type");
		return;
	}

	try {
		m_map.SetBlock(clientBlock.x, clientBlock.y, clientBlock.z, type);
	} catch(std::runtime_error const& e) {
		LOG(LogLevel::kWarning, "Exception in LuaPlaceBlock: %s", e.what());

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

	m_saveFlag = true;
}

void World::BroadcastMessage(std::string message)
{
	for (auto& obj : m_clients)
		Server::GetInstance()->SendWrappedMessage(obj, "&e[WORLD]: " + message);
}

void World::SendBlockToClients(uint8_t type, short x, short y, short z)
{
	for (auto& obj : m_clients)
		Protocol::SendBlock(obj, Position(x, y, z), type);
}
