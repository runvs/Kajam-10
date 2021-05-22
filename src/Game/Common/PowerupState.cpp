#include "PowerupState.hpp"
#include "Conversions.hpp"
#include "GameProperties.hpp"

sf::Packet& operator<<(sf::Packet& packet, PowerupState& powerupState)
{
    packet << powerupState.position;
    packet << powerupState.type;
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, PowerupState const& powerupState)
{
    packet << powerupState.position;
    packet << powerupState.type;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, PowerupState& powerupState)
{
    packet >> powerupState.position;
    packet >> powerupState.type;
    return packet;
}

void updatePowerupState(PowerupState& powerup, float elapsed)
{
    powerup.position.y() += elapsed * Game::GameProperties::scrollSpeed();

    powerup._alive = (powerup.position.y() < Game::GameProperties::displayScreenSize().y() + 32);
}
