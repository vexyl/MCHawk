#include "CPE.hpp"

bool CPE::IsValidBlock(int type)
{
	for (int i = BlockType::kStartOfBlockTypes; i < BlockType::kEndOfBlockTypes; ++i) {
		if (type == i)
			return true;
	}

	return false;
}

void CPE::SendExtInfo(Client* client, std::string appName, short extCount)
{
		Packet packet(CPE::PacketType::kExtInfo);
		packet.Write(appName);
		packet.Write(extCount);
		client->QueuePacket(packet);
}

void CPE::SendExtEntry(Client* client, std::string extName, int version)
{
		Packet packet(CPE::PacketType::kExtEntry);
		packet.Write(extName);
		packet.Write(version);
		client->QueuePacket(packet);
}

void CPE::SendCustomBlocks(Client* client, uint8_t support)
{
		Packet packet(CPE::PacketType::kCustomBlocks);
		packet.Write(support);
		client->QueuePacket(packet);
}
