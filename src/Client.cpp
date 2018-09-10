#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>

#include <string>

namespace Utils {

// http://www.cplusplus.com/reference/ctime/strftime/
// https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
const std::string CurrentDate();
const std::string CurrentTime();

int32_t Distance3d(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);
bool DistanceCheck(int32_t distance, int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);

unsigned int GetRandomUInt(unsigned int n);
std::string GetRandomSalt();#include "Client.hpp"
#include "World.hpp"

#include "Utils/Logger.hpp"

uint8_t Client::pid = 0;

Client::Client() : m_pid(pid++), m_world(nullptr), m_userType(0), m_yaw(0), m_pitch(0), m_chatMuteTime(0)
{
  active = false;
  authed = false;
}

Client::~Client()
{
  delete stream.socket;

  for (auto& obj : m_packetQueue)
    delete obj;
}

// Checks if player is an operator or if the world allows building
bool Client::CanBuild()
{
  return (m_userType == 0x64) || (m_world->GetOption("build") == "true");
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

void Client::SetPositionOrientation(Position position, uint8_t yaw, uint8_t pitch)
{
  m_position = position;
  m_yaw = yaw;
  m_pitch = pitch;
}

void Client::SetChatMute(int32_t chatMuteTime)
{
  m_chatMuteTime = chatMuteTime;
  m_chatMuteClock.restart();
}

bool Client::IsChatMuted()
{
  if (m_chatMuteTime > 0 && m_chatMuteClock.getElapsedTime().asMilliseconds() >= m_chatMuteTime)
    SetChatMute(0); // disable chat mute by setting timer to 0ms

  return m_chatMuteTime > 0;
}

void Client::QueuePacket(Packet* packet)
{
  m_packetQueue.push_back(packet);
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

      break; // No threading yet, so we don't want to process every packet in queue right away
    }

    // Packet processed successfully, move on
    delete *it;
    it = m_packetQueue.erase(it);
  }
}


} // namespace Utils

#endif // UTILS_H_
