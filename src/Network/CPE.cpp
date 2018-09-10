#include "CPE.hpp"

bool CPE::IsValidBlock(uint8_t type)
{
  for (int i = BlockType::kStartOfBlockTypes; i < BlockType::kEndOfBlockTypes; ++i) {
    if (type == i)
      return true;
  }

  return false;
}

void CPE::SendExtInfo(Client* client, std::string appName, short extCount)
{
    Packet* packet = new Packet(CPE::PacketType::kExtInfo);

    packet->Write(appName);
    packet->Write(extCount);

    client->QueuePacket(packet);
}

void CPE::SendExtEntry(Client* client, std::string extName, int version)
{
    Packet* packet = new Packet(CPE::PacketType::kExtEntry);

    packet->Write(extName);
    packet->Write(version);

    client->QueuePacket(packet);
}

void CPE::SendCustomBlocks(Client* client, uint8_t support)
{
    Packet* packet = new Packet(CPE::PacketType::kCustomBlocks);

    packet->Write(support);

    client->QueuePacket(packet);
}
