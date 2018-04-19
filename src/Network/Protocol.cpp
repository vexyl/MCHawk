#include "Protocol.hpp"

#include "Packet.hpp"
#include "../Map.hpp"
#include "../Utils/Logger.hpp"

void make_spawn_packet(Packet& packet, int8_t pid, std::string name, Position position, int8_t yaw, int8_t pitch)
{
	packet.Write((int8_t)SSPAWN);
	packet.Write(pid); // Self PID
	packet.Write(name);
	packet.Write(htons(position.x));
	packet.Write(htons(position.y));
	packet.Write(htons(position.z));
	packet.Write((uint8_t)yaw); // yaw
	packet.Write((uint8_t)pitch); // pitch
}

void SendInfo(Client* client, std::string serverName, std::string serverMOTD, uint8_t version, uint8_t userType)
{
	Packet packet(SSINFO);

	packet.Write((uint8_t)version);
	packet.Write(serverName);
	packet.Write(serverMOTD);
	packet.Write((uint8_t)userType);

	client->QueuePacket(packet);
}

void SendMessage(Client* client, std::string message)
{
	Packet packet(SMSG);

	packet.Write((uint8_t)0);
	packet.Write(message);

	client->QueuePacket(packet);
}

void SendMap(Client* client, Map& map)
{
	Packet levelInitPacket(SLINIT);
	client->QueuePacket(levelInitPacket);

	uint8_t* compBuffer = nullptr;
	size_t compSize;

	map.CompressBuffer(&compBuffer, &compSize);

	LOG(DEBUG, "Compressed map size: %d bytes", compSize);

	size_t bytes = 0;
	while (bytes < compSize) {
		size_t remainingBytes = compSize - bytes;
		size_t count = (remainingBytes >= 1024) ? 1024 : (remainingBytes);

		Packet packet(SLDATA);

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

	Packet packet(SLFINAL);

	packet.Write(htons(mapX));
	packet.Write(htons(mapY));
	packet.Write(htons(mapZ));

	client->QueuePacket(packet);
}

void SendBlock(Client* client, Position pos, uint8_t type)
{
	Packet packet(SBLOCK);

	packet.Write(htons(pos.x));
	packet.Write(htons(pos.y));
	packet.Write(htons(pos.z));
	packet.Write(type);

	client->QueuePacket(packet);
}

void SendKick(Client* client, std::string reason)
{
	Packet packet(SKICK);

	packet.Write(reason);

	client->QueuePacket(packet);
}

void SendPosition(Client* client, int8_t pid, Position pos, uint8_t yaw, uint8_t pitch)
{
	Packet packet(STELE);

	packet.Write(pid);
	packet.Write(htons(pos.x));
	packet.Write(htons(pos.y));
	packet.Write(htons(pos.z));
	packet.Write(yaw);
	packet.Write(pitch);

	client->QueuePacket(packet);
}

void SendPlayerPositionUpdate(Client* sender, const std::vector<Client*>& clients)
{
	// Do this on a timer in Tick()
	// For now this will work
	int8_t pid = sender->GetPid();
	Position pos = sender->GetPosition();

	Packet packet(STELE);
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

void SendUserType(Client* client, uint8_t userType)
{
	Packet packet(SUTYPE);

	packet.Write(userType);

	client->QueuePacket(packet);
}

void SendClientsTo(Client* client, const std::vector<Client*>& clients)
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

void SpawnClient(Client* client, Position position, const std::vector<Client*>& clients)
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

void DespawnClient(int8_t pid, const std::vector<Client*>& clients)
{
	Packet packet(SDESPAWN);

	packet.Write(pid);

	for (auto& obj : clients)
		obj->QueuePacket(packet);
}
