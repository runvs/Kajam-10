﻿#include "ShotState.hpp"
#include "Conversions.hpp"
#include "GameProperties.hpp"

sf::Packet& operator<<(sf::Packet& packet, ShotState& shotState)
{
    return packet << shotState.position << shotState.direction << shotState.fromPlayer;
}

sf::Packet& operator<<(sf::Packet& packet, ShotState const& shotState)
{
    return packet << shotState.position << shotState.direction << shotState.fromPlayer;
}

sf::Packet& operator>>(sf::Packet& packet, ShotState& shotState)
{
    return packet >> shotState.position >> shotState.direction >> shotState.fromPlayer;
}

void updateShotState(ShotState& s, float elapsed)
{
    s.position += s.direction * elapsed * Game::GameProperties::shotVelocity();
    s._age += elapsed;
}
