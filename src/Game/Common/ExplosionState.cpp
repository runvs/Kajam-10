#include "ExplosionState.hpp"
#include "Conversions.hpp"

sf::Packet& operator<<(sf::Packet& packet, ExplosionState& explosionState)
{
    packet << explosionState.position;
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, ExplosionState const& explosionState)
{
    packet << explosionState.position;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, ExplosionState& explosionState)
{
    packet >> explosionState.position;
    return packet;
}