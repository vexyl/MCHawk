#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "../Client.hpp"

#include <string>
#include <vector>

#include <stdint.h>

#ifdef __linux__
	#include <arpa/inet.h>
#elif _WIN32
	#include <winsock2.h>
#endif

#include "ClientStream.hpp"

#include "Packet.hpp"
#include "../Position.hpp"

/* Server->Client Opcodes */
#define SSINFO 0x00	/* Server information: Name, MOTD */
#define SPING 0x01	/* Needs no reply */
#define SLINIT 0x02	/* Level initialized, data incoming */
#define SLDATA 0x03	/* Map data chunk, sent after SINIT */
#define SLFINAL 0x04	/* Level finalized, map dimensions */
#define SMSG 0x0d	/* Any chat message */
#define SKICK 0x0e	/* Server kick, gives reason */
#define SSPAWN 0x07	/* Player spawned */
#define SDESPAWN 0x0c	/* Player despawned */
#define STELE 0x08	/* Player teleported */
#define SPOSDTDIR 0x09	/* Position/Direction change */
#define SPOSDT 0x0a	/* Position change */
#define SDIR 0x0b	/* Direction change */
#define SBLOCK 0x06	/* Block change */
#define SUTYPE 0x0f	/* Player (de)opped */

/* Client->Server Opcodes */
#define CAUTH 0x00	/* Client authenitcation */
#define CMSG 0x0d	/* Client messsages */
#define CPOS 0x08	/* Client position update */
#define CBLOCK 0x05	/* Client block change */

/* Client->Server */
struct cauthp {
	uint8_t opcode;
	uint8_t version;
	uint8_t name[64];
	uint8_t key[64];
	uint8_t UNK0;

	bool Read(ClientStream& stream)
	{
		uint8_t buffer[131];
		if (!stream.consume_data(buffer, sizeof(buffer))) return false;

		Packet packet;
		packet.Write(buffer, sizeof(buffer));

		packet.Read(UNK0);
		packet.Read(key, sizeof(key));
		packet.Read(name, sizeof(name));
		packet.Read(version);
		//packet.Read(opcode);

		return true;
	}
};

struct cmsgp {
	uint8_t opcode;
	uint8_t flag;
	uint8_t msg[64];

	bool Read(ClientStream& stream)
	{
		uint8_t buffer[66];
		if (!stream.consume_data(buffer, sizeof(buffer))) return false;

		Packet packet;
		packet.Write(buffer, sizeof(buffer));

		packet.Read(msg, sizeof(msg));
		packet.Read(flag);
		//packet.Read(opcode);

		return true;
	}
};

struct cposp {
	uint8_t opcode;
	uint8_t pid;
	short x, y, z;
	uint8_t yaw, pitch;

	bool Read(ClientStream& stream)
	{
		uint8_t buffer[10];
		if (!stream.consume_data(buffer, sizeof(buffer))) return false;

		Packet packet;
		packet.Write(buffer, sizeof(buffer));

		packet.Read(pitch);
		packet.Read(yaw);
		packet.Read(z);
		packet.Read(y);
		packet.Read(x);
		packet.Read(pid);
		//packet.Read(opcode);

		x = ntohs(x);
		y = ntohs(y);
		z = ntohs(z);

		return true;
	}
};

struct cblockp {
	uint8_t opcode;
	short x, y, z;
	uint8_t mode;
	uint8_t type;

	bool Read(ClientStream& stream)
	{
		uint8_t buffer[9];
		if (!stream.consume_data(buffer, sizeof(buffer))) return false;

		Packet packet;
		packet.Write(buffer, sizeof(buffer));

		packet.Read(type);
		packet.Read(mode);
		packet.Read(z);
		packet.Read(y);
		packet.Read(x);
		//packet.Read(opcode);

		x = ntohs(x);
		y = ntohs(y);
		z = ntohs(z);

		return true;
	}
};

void make_spawn_packet(Packet& packet, int8_t pid, std::string name, Position position, int8_t yaw, int8_t pitch);

// Send packet functions
void SendInfo(Client* client, std::string serverName, std::string serverMOTD, uint8_t version, uint8_t userType=0);
void SendMessage(Client* client, std::string message);
void SendMap(Client* client, Map& map);
void SendBlock(Client* client, Position pos, uint8_t type);
void SendKick(Client* client, std::string reason="");
void SendPosition(Client* client, int8_t pid, Position pos, uint8_t yaw, uint8_t pitch);
void SendPlayerPositionUpdate(Client* sender, const std::vector<Client*>& clients);
void SendUserType(Client* client, uint8_t userType);
void SendClientsTo(Client* client, const std::vector<Client*>& clients);
void SpawnClient(Client* client, Position position, const std::vector<Client*>& clients);
void DespawnClient(int8_t pid, const std::vector<Client*>& clients);

#endif // PROTOCOL_H_
