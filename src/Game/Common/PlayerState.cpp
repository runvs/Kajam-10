﻿#include "PlayerState.hpp"
#include "Conversions.hpp"
#include "EnemyState.hpp"
#include "GameProperties.hpp"
#include "MathHelper.hpp"
#include "ShotState.hpp"

sf::Packet& operator<<(sf::Packet& packet, PlayerState& playerState)
{
    packet << playerState.position;
    packet << playerState.health;
    return packet;
}
sf::Packet& operator<<(sf::Packet& packet, PlayerState const& playerState)
{

    packet << playerState.position;
    packet << playerState.health;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerState& playerState)
{

    packet >> playerState.position;
    packet >> playerState.health;
    return packet;
}

void updatePlayerState(PlayerState& playerState, float elapsed, InputState& input)
{
    if (input[jt::KeyCode::D])
        playerState.position.x() += elapsed * Game::GameProperties::playerMovementSpeed();
    else if (input[jt::KeyCode::A])
        playerState.position.x() -= elapsed * Game::GameProperties::playerMovementSpeed();

    if (input[jt::KeyCode::W])
        playerState.position.y() -= elapsed * Game::GameProperties::playerMovementSpeed();
    else if (input[jt::KeyCode::S])
        playerState.position.y() += elapsed * Game::GameProperties::playerMovementSpeed();

    playerState.position.x() = jt::MathHelper::clamp(playerState.position.x(), 0.0f,
        Game::GameProperties::displayScreenSize().x()
            - static_cast<float>(Game::GameProperties::playerSizeInPixel()));
    playerState.position.y() = jt::MathHelper::clamp(playerState.position.y(), 0.0f,
        Game::GameProperties::displayScreenSize().y()
            - static_cast<float>(Game::GameProperties::playerSizeInPixel()));

    playerState._shootTimer -= elapsed;
}

void playerTakeDamage(PlayerState& playerState, ShotState& shot)
{
    playerState.health -= shot._damage;
}

// TODO take enemy size into account
void playerTakeDamage(PlayerState& playerState, EnemyState& enemy) { playerState.health -= 4; }
