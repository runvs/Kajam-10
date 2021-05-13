#include "ShotState.hpp"
#include "Conversions.hpp"

sf::Packet& operator<<(sf::Packet& packet, ShotState& shotState)
{
    return packet << shotState.position << shotState.direction;
}

sf::Packet& operator<<(sf::Packet& packet, ShotState const& shotState)
{
    return packet << shotState.position << shotState.direction;
}

sf::Packet& operator>>(sf::Packet& packet, ShotState& shotState)
{
    return packet >> shotState.position >> shotState.direction;
}
