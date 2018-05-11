#ifndef CLIENT_H_
#define CLIENT_H_

#include <string.h>
#include <assert.h>

#include <string>

#include "Network/ClientStream.hpp"
#include "Network/Packet.hpp"
#include "Map.hpp"
#include "Position.hpp"

class World;

class Client {
public:
	ClientStream stream;
	bool active;
	bool authed;
	bool kicked;

	Client();

	~Client();

	bool CanBuild();

	std::string GetIpString();
	std::string GetName() const { return m_name; }
	std::string GetChatName() { return m_chatName; }
	int8_t GetPid() { return m_pid; }
	uint8_t GetUserType() { return m_userType; }

	Position GetPosition() { return m_position; }
	uint8_t GetYaw() { return m_yaw; }
	uint8_t GetPitch() { return m_pitch; }
	World* GetWorld() { return m_world; }

	bool IsActive() { return active; }

	void SetName(std::string name) { m_name = name; }
	void SetChatName(std::string name) { m_chatName = name; }
	void SetPositionOrientation(Position position, uint8_t yaw, uint8_t pitch);
	void SetUserType(uint8_t userType) { m_userType = userType; }
	void SetChatMute(int32_t chatMuteTime=0);
	void SetWorld(World* world) { m_world = world; }

	bool IsChatMuted();

	void QueuePacket(Packet* packet);

	void ProcessPacketsInQueue();

private:
	static int8_t pid;

	std::string m_name;
	std::string m_chatName;
	std::string m_ipString;
	uint8_t m_pid;

	uint8_t m_userType;
	Position m_position;
	uint8_t m_yaw, m_pitch;

	std::vector<Packet*> m_packetQueue;

	sf::Clock m_chatMuteClock;
	bool m_chatMuted;
	int32_t m_chatMuteTime;

	std::string m_worldName;
	World* m_world;
};

#endif // CLIENT_H_
