#include "PlayerState.hpp"
#include "Conversions.hpp"
#include "EnemyState.hpp"
#include "GameProperties.hpp"
#include "MathHelper.hpp"
#include "ShotState.hpp"
#include <iostream>

sf::Packet& operator<<(sf::Packet& packet, PlayerState& playerState)
{
    packet << playerState.position;
    packet << playerState.health;
    packet << playerState.pickedUpPowerup;
    packet << playerState.takenDamage;
    return packet;
}
sf::Packet& operator<<(sf::Packet& packet, PlayerState const& playerState)
{

    packet << playerState.position;
    packet << playerState.health;
    packet << playerState.pickedUpPowerup;
    packet << playerState.takenDamage;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerState& playerState)
{
    packet >> playerState.position;
    packet >> playerState.health;
    packet >> playerState.pickedUpPowerup;
    packet >> playerState.takenDamage;
    return packet;
}

void respawnPlayer(PlayerState& playerState)
{
    playerState._gracePeriod = Game::GameProperties::playerGracePeriodAfterDeath();
    playerState._respawnTimer = 0;
    playerState._shotPattern = Shots::ShotPattern::level1();
    playerState.health = Game::GameProperties::playerMaxHealth();
}

void updatePlayerState(PlayerState& playerState, float elapsed, InputState const& input)
{
    float const factor = input.at(jt::KeyCode::Space)
        ? Game::GameProperties::playerMovementSpeedSlowdownIfShooting()
        : 1.0f;
    const float movementSpeed = elapsed * Game::GameProperties::playerMovementSpeed() * factor;

    if (playerState.health > 0) {
        if (input.at(jt::KeyCode::D))
            playerState.position.x() += movementSpeed;
        else if (input.at(jt::KeyCode::A))
            playerState.position.x() -= movementSpeed;

        if (input.at(jt::KeyCode::W))
            playerState.position.y() -= movementSpeed;
        else if (input.at(jt::KeyCode::S))
            playerState.position.y() += movementSpeed;

        auto const minXPos
            = Game::GameProperties::hudBarMargin() * 2.0f + Game::GameProperties::hudBarWidth();
        auto const maxXPos = Game::GameProperties::displayScreenSize().x()
            - static_cast<float>(Game::GameProperties::playerSizeInPixel())
            - Game::GameProperties::hudBarWidth() - Game::GameProperties::hudBarMargin() * 2.0f;

        auto const minYPos = Game::GameProperties::hudBarMargin();
        auto const maxYPos = Game::GameProperties::displayScreenSize().y()
            - static_cast<float>(Game::GameProperties::playerSizeInPixel())
            - Game::GameProperties::hudBarMargin();

        playerState.position.x()
            = jt::MathHelper::clamp(playerState.position.x(), minXPos, maxXPos);
        playerState.position.y()
            = jt::MathHelper::clamp(playerState.position.y(), minYPos, maxYPos);

        playerState._shootTimer -= elapsed;
        playerState._gracePeriod -= elapsed;
    } else {
        playerState._respawnTimer -= elapsed;
        if (playerState._respawnTimer <= 0) {
            respawnPlayer(playerState);
        }
    }
}

void killPlayer(PlayerState& playerState)
{
    playerState._respawnTimer = Game::GameProperties::playerRespawnTime();
}

void checkPlayerDeath(PlayerState& playerState)
{
    if (playerState.health <= 0) {
        killPlayer(playerState);
    }
}
void playerTakeDamageImpl(PlayerState& playerState, int damage)
{
    if (playerState.health <= 0 || playerState._gracePeriod >= 0.0f) {
        return;
    }
    playerState.health -= damage;
    playerState.takenDamage = true;
    checkPlayerDeath(playerState);
}

void playerTakeDamage(PlayerState& playerState, ShotState& shot)
{
    playerTakeDamageImpl(playerState, shot._damage);
}

// TODO take enemy size into account
void playerTakeDamage(PlayerState& playerState, EnemyState& enemy)
{
    playerTakeDamageImpl(playerState, Game::GameProperties::playerEnemyCollisionDamage());
}
