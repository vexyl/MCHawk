#ifndef CPE_H_
#define CPE_H_

#include "../Client.hpp"

#include <cstdint>

#include "ClientStream.hpp"
#include "Packet.hpp"

namespace CPE {

enum PacketType {
	kExtInfo = 0x10,
	kExtEntry = 0x11,
	kCustomBlocks = 0x13
};

enum BlockType {
	kStartOfBlockTypes=0x32,
	kCobblestoneSlab=kStartOfBlockTypes,
	kRope,
	kSandstone,
	kSnow,
	kFire,
	kLightPinkWool,
	kForestGreenWool,
	kBrownWool,
	kDeepBlue,
	kTurquoise,
	kIce,
	kCeramicTile,
	kMagma,
	kPillar,
	kCrate,
	kStoneBrick,
	kEndOfBlockTypes
};

struct cextinfop {
	uint8_t opcode;
	std::string appName;
	short extCount;

	bool Read(ClientStream& stream)
	{
		uint8_t buffer[67];
		if (!stream.consume_data(buffer, sizeof(buffer))) return false;

		Packet packet;
		packet.Write(buffer, sizeof(buffer));

		packet.Read(extCount);
		packet.Read(appName);
		//packet.Read(opcode);

		return true;
	}
};

struct cextentryp {
	uint8_t opcode;
	std::string extName;
	short version;

	bool Read(ClientStream& stream)
	{
		uint8_t buffer[69];
		if (!stream.consume_data(buffer, sizeof(buffer))) return false;

		Packet packet;
		packet.Write(buffer, sizeof(buffer));

		packet.Read(version);
		packet.Read(extName);
		//packet.Read(opcode);

		return true;
	}
};

struct ccustomblockp {
	uint8_t opcode;
	uint8_t support;

	bool Read(ClientStream& stream)
	{
		uint8_t buffer[2];
		if (!stream.consume_data(buffer, sizeof(buffer))) return false;

		Packet packet;
		packet.Write(buffer, sizeof(buffer));

		packet.Read(support);
		//packet.Read(opcode);

		return true;
	}
};

bool IsValidBlock(uint8_t type);

// Send packet functions
void SendExtInfo(Client* client, std::string appName, short extCount);
void SendExtEntry(Client* client, std::string extName, int version);
void SendCustomBlocks(Client* client, uint8_t support);

} // namespace CPE

#endif // CPE_H_
