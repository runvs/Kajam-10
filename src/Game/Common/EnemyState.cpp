#include "EnemyState.hpp"
#include "Conversions.hpp"

sf::Packet& operator<<(sf::Packet& packet, EnemyState& enemyState)
{
    return packet << enemyState.position;
}

sf::Packet& operator<<(sf::Packet& packet, EnemyState const& enemyState)
{
    return packet << enemyState.position;
}

sf::Packet& operator>>(sf::Packet& packet, EnemyState& enemyState)
{
    return packet >> enemyState.position;
}