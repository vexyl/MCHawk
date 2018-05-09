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

namespace Protocol {
enum PacketType {
	kServerInfo				= 0x00,
	kServerPing				= 0x01,
	kServerLevelInit				= 0x02,
	kServerLevelData			= 0x03,
	kServerLevelFinal			= 0x04,
	kServerMessage				= 0x0d,
	kServerKick				= 0x0e,
	kServerSpawn				= 0x07,
	kServerDespawn				= 0x0c,
	kServerTeleport				= 0x08,
	kServerPositionOrientationChange	= 0x09,
	kServerPositionChange			= 0x0a,
	kServerDirection			= 0x0b,
	kServerBlock				= 0x06,
	kServerUserType				= 0x0f,
	kClientAuth				= 0x00,
	kClientMessage				= 0x0d,
	kClientPosition				= 0x08,
	kClientBlock				= 0x05
};

enum BlockType {
	kStartOfBlockTypes=0x00,
	kAir=kStartOfBlockTypes,
	kStone,
	kGrass,
	kDirt,
	kCobblestone,
	kWoodPlanks,
	kSapling,
	kBedrock,
	kFlowingWater,
	kStationaryWater,
	kFlowingLava,
	kStationaryLava,
	kSand,
	kGravel,
	kGoldOre,
	kIronOre,
	kCoalOre,
	kWood,
	kLeaves,
	kSponge,
	kGlass,
	kRedCloth,
	kOrangeCloth,
	kYellowCloth,
	kLimeCloth,
	kGreenCloth,
	kAquaGreenCloth,
	kCyanCloth,
	kBlueCloth,
	kPurpleCloth,
	kIndigoCloth,
	kVioletCloth,
	kMagentaCloth,
	kPinkCloth,
	kBlackCloth,
	kGrayCloth,
	kWhiteCloth,
	kDandelion,
	kRose,
	kBrownMushroom,
	kRedMushroom,
	kGoldBlock,
	kIronBlock,
	kDoubleSlab,
	kSlab,
	kBricks,
	kTNT,
	kBookshelf,
	kMossStone,
	kObsidian,
	kEndOfBlockTypes
};

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

bool IsValidBlock(int type);

Packet* make_spawn_packet(int8_t pid, std::string name, Position position, int8_t yaw, int8_t pitch);

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
} // namespace Protocol

#endif // PROTOCOL_H_
