#include "Client.hpp"

#include "Utils/Logger.hpp"

int8_t Client::pid = 0;

Client::Client() : m_world(nullptr)
{
	m_pid = pid++;
	m_userType = 0;
	m_yaw = m_pitch = 0;

	m_chatMuted = false;
}

std::string Client::GetIpString()
{
	if (m_ipString.empty()) {
		assert(stream.socket != nullptr);

		sf::IpAddress address = stream.socket->getRemoteAddress();
		m_ipString = "Unknown";

		if (address != sf::IpAddress::None)
			m_ipString = address.toString();
	}

	return m_ipString;
}

void Client::SetName(std::string name)
{
	if (m_chatName.empty())
		m_chatName = name;

	m_name = name;
}

void Client::SetPositionOrientation(Position position, uint8_t yaw, uint8_t pitch)
{
	m_position = position;
	m_yaw = yaw;
	m_pitch = pitch;
}

void Client::SetChatMute(int32_t chatMuteTime)
{
	m_chatMuted = (chatMuteTime == 0) ? 0 : 1;
	m_chatMuteTime = chatMuteTime;

	m_chatMuteClock.restart();
}

bool Client::IsChatMuted()
{
	if (m_chatMuted && m_chatMuteTime > 0 && m_chatMuteClock.getElapsedTime().asMilliseconds() >= m_chatMuteTime) {
		SetChatMute(false);
		m_chatMuted = false;
	}

	return m_chatMuted;
}

void Client::QueuePacket(Packet& packet)
{
	// I know this is atrocious code
	Packet* p = new Packet();
	p->Write(packet.GetBufferPtr(), packet.GetLength());
	m_packetQueue.push_back(p);
}

void Client::ProcessPacketsInQueue()
{
	auto it = m_packetQueue.begin();
	while (it != m_packetQueue.end()) {
		// send() may send packet partially and then return NotReady
		size_t sent = 0;
		auto status = stream.socket->send((*it)->GetBufferPtr(), (*it)->GetLength(), sent);

		if (status != sf::TcpSocket::Status::Done) {
			//LOG(DEBUG, "Failed to send packet->%s (sent=%d)", m_name.c_str(), sent);

			if (sent > 0) {
				// Sent partial packet, get the rest and queue it
				Packet* p = new Packet();
				p->Write((*it)->GetBufferPtr() + sent, (*it)->GetLength() - sent);

				// Replace old packet with new partial
				delete *it;
				*it = p;
			}

			break;
		}

		// Packet processed successfully, move on
		delete *it;
		it = m_packetQueue.erase(it);
	}
}
