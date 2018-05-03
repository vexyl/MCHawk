#include "Protocol.hpp"

#include "Packet.hpp"
#include "../Map.hpp"
#include "../Utils/Logger.hpp"

void ClassicProtocol::make_spawn_packet(Packet& packet, int8_t pid, std::string name, Position position, int8_t yaw, int8_t pitch)
{
	// Cast opcode because it's an initialized packet and we're writing the packet type, not constructing the packet
	packet.Write((uint8_t)ClassicProtocol::PacketType::kServerSpawn);
	packet.Write(pid); // Self PID
	packet.Write(name);
	packet.Write(htons(position.x));
	packet.Write(htons(position.y));
	packet.Write(htons(position.z));
	packet.Write((uint8_t)yaw); // yaw
	packet.Write((uint8_t)pitch); // pitch
}

void ClassicProtocol::SendInfo(Client* client, std::string serverName, std::string serverMOTD, uint8_t version, uint8_t userType)
{
	Packet packet(ClassicProtocol::PacketType::kServerInfo);

	packet.Write((uint8_t)version);
	packet.Write(serverName);
	packet.Write(serverMOTD);
	packet.Write((uint8_t)userType);

	client->QueuePacket(packet);
}

void ClassicProtocol::SendMessage(Client* client, std::string message)
{
	Packet packet(ClassicProtocol::PacketType::kServerMessage);

	packet.Write((uint8_t)0);
	packet.Write(message);

	client->QueuePacket(packet);
}

void ClassicProtocol::SendMap(Client* client, Map& map)
{
	Packet levelInitPacket(ClassicProtocol::PacketType::kServerInit);
	client->QueuePacket(levelInitPacket);

	uint8_t* compBuffer = nullptr;
	size_t compSize;

	map.CompressBuffer(&compBuffer, &compSize);

	LOG(LogLevel::kDebug, "Compressed map size: %d bytes", compSize);

	size_t bytes = 0;
	while (bytes < compSize) {
		size_t remainingBytes = compSize - bytes;
		size_t count = (remainingBytes >= 1024) ? 1024 : (remainingBytes);

		Packet packet(ClassicProtocol::PacketType::kServerLevelData);

		packet.Write((int16_t)htons(count)); // length

		packet.BufferStream::Write((void*)(&compBuffer[bytes]), count);
		// Padding; must send exactly 1024 bytes per chunk
		if (count < 1024) {
			size_t paddingSize = 1024 - count;
			packet.WriteZeroes(paddingSize);
		}

		bytes += count;

		char percent = (char)(((float)bytes / (float)compSize) * 100.0f);
		packet.Write((uint8_t)percent);

		//printf("%d/%d bytes | %d%%\n", (int)bytes, (int)bufferSize, (int)percent); // Debug

		client->QueuePacket(packet);
	}

	free(compBuffer);

	short mapX = map.GetXSize();
	short mapY = map.GetYSize();
	short mapZ = map.GetZSize();

	Packet packet(ClassicProtocol::PacketType::kServerLevelFinal);

	packet.Write(htons(mapX));
	packet.Write(htons(mapY));
	packet.Write(htons(mapZ));

	client->QueuePacket(packet);
}

void ClassicProtocol::SendBlock(Client* client, Position pos, uint8_t type)
{
	Packet packet(ClassicProtocol::PacketType::kServerBlock);

	packet.Write(htons(pos.x));
	packet.Write(htons(pos.y));
	packet.Write(htons(pos.z));
	packet.Write(type);

	client->QueuePacket(packet);
}

void ClassicProtocol::SendKick(Client* client, std::string reason)
{
	Packet packet(ClassicProtocol::PacketType::kServerKick);

	packet.Write(reason);

	client->QueuePacket(packet);
}

void ClassicProtocol::SendPosition(Client* client, int8_t pid, Position pos, uint8_t yaw, uint8_t pitch)
{
	Packet packet(ClassicProtocol::PacketType::kServerTeleport);

	packet.Write(pid);
	packet.Write(htons(pos.x));
	packet.Write(htons(pos.y));
	packet.Write(htons(pos.z));
	packet.Write(yaw);
	packet.Write(pitch);

	client->QueuePacket(packet);
}

void ClassicProtocol::SendPlayerPositionUpdate(Client* sender, const std::vector<Client*>& clients)
{
	// Do this on a timer in Tick()
	// For now this will work
	int8_t pid = sender->GetPid();
	Position pos = sender->GetPosition();

	Packet packet(ClassicProtocol::PacketType::kServerTeleport);
	packet.Write(pid);
	packet.Write(htons(pos.x));
	packet.Write(htons(pos.y));
	packet.Write(htons(pos.z));
	packet.Write(sender->GetYaw());
	packet.Write(sender->GetPitch());

	for (auto& obj : clients) {
		if (obj->GetPid() != pid)
			obj->QueuePacket(packet);
	}
}

void ClassicProtocol::SendUserType(Client* client, uint8_t userType)
{
	Packet packet(ClassicProtocol::PacketType::kServerUserType);

	packet.Write(userType);

	client->QueuePacket(packet);
}

void ClassicProtocol::SendClientsTo(Client* client, const std::vector<Client*>& clients)
{
	int8_t clientPid = client->GetPid();
	for (auto& obj : clients) {
		int8_t pid = obj->GetPid();

		if (pid == clientPid) continue;

		Packet packet;
		make_spawn_packet(packet, pid, obj->GetName(), obj->GetPosition(), obj->GetYaw(), obj->GetPitch());
		client->QueuePacket(packet);
	}
}

void ClassicProtocol::SpawnClient(Client* client, Position position, const std::vector<Client*>& clients)
{
	std::string name = client->GetName();
	int8_t pid = -1; // Self PID first

	client->SetPositionOrientation(position, 0, 0);

	Packet selfSpawnPacket;
	make_spawn_packet(selfSpawnPacket, pid, name, position, 0, 0);
	client->QueuePacket(selfSpawnPacket);

	pid = client->GetPid(); // Actual PID

	Packet spawnPacket;
	make_spawn_packet(spawnPacket, pid, name, position, 0, 0);
	// Send client spawn to rest of clients
	for (auto& obj : clients) {
		if (obj->GetPid() != pid)
			obj->QueuePacket(spawnPacket);
	}
}

void ClassicProtocol::DespawnClient(int8_t pid, const std::vector<Client*>& clients)
{
	Packet packet(ClassicProtocol::PacketType::kServerDespawn);

	packet.Write(pid);

	for (auto& obj : clients)
		obj->QueuePacket(packet);
}
