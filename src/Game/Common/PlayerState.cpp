#include "PlayerState.hpp"
#include "Conversions.hpp"

sf::Packet& operator<<(sf::Packet& packet, PlayerState& playerState)
{
    return packet << playerState.position;
}
sf::Packet& operator<<(sf::Packet& packet, PlayerState const& playerState)
{
    return packet << playerState.position;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerState& playerState)
{
    return packet >> playerState.position;
}
