#include "Payloads.hpp"
#include "Conversions.hpp"
#include "PlayerState.hpp"

sf::Packet& operator<<(sf::Packet& packet, PayloadClient2Server& payload)
{
    return packet << payload.playerID << payload.input;
}

sf::Packet& operator>>(sf::Packet& packet, PayloadClient2Server& payload)
{
    return packet >> payload.playerID >> payload.input;
}

sf::Packet& operator<<(sf::Packet& packet, PayloadServer2Client& payload)
{
    return packet << payload.playerID << payload.playerStates;
}
sf::Packet& operator>>(sf::Packet& packet, PayloadServer2Client& payload)
{
    return packet >> payload.playerID >> payload.playerStates;
}
