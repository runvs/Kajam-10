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

void respawnPlayer(PlayerState& playerState)
{
    playerState._respawnTimer = 0;
    playerState._shotPattern = Shots::ShotPattern::SINGLE();
    playerState.health = Game::GameProperties::playerMaxHealth();
}

void updatePlayerState(PlayerState& playerState, float elapsed, InputState const& input)
{
    if (playerState.health > 0) {
        if (input.at(jt::KeyCode::D))
            playerState.position.x() += elapsed * Game::GameProperties::playerMovementSpeed();
        else if (input.at(jt::KeyCode::A))
            playerState.position.x() -= elapsed * Game::GameProperties::playerMovementSpeed();

        if (input.at(jt::KeyCode::W))
            playerState.position.y() -= elapsed * Game::GameProperties::playerMovementSpeed();
        else if (input.at(jt::KeyCode::S))
            playerState.position.y() += elapsed * Game::GameProperties::playerMovementSpeed();

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

        // TODO remove following pattern test code
        // Set up some toggles for shot patterns
        if (playerState._patternToggleTimer <= 0.0f) {
            const float patternTimerMax = 0.2f;
            if (input.at(jt::KeyCode::Num1)) {
                playerState._shotPattern ^= Shots::ShotPattern::SINGLE();
                if ((playerState._shotPattern & Shots::ShotPattern::SINGLE())
                    == Shots::ShotPattern::SINGLE())
                    std::cout << "ShotPattern SINGLE: ON" << std::endl;
                else
                    std::cout << "ShotPattern SINGLE: OFF" << std::endl;
                playerState._patternToggleTimer = patternTimerMax;
            } else if (input.at(jt::KeyCode::Num2)) {
                playerState._shotPattern ^= Shots::ShotPattern::TRIPLE_NARROW();
                if ((playerState._shotPattern & Shots::ShotPattern::TRIPLE_NARROW())
                    == Shots::ShotPattern::TRIPLE_NARROW())
                    std::cout << "ShotPattern TRIPLE_NARROW: ON" << std::endl;
                else
                    std::cout << "ShotPattern TRIPLE_NARROW: OFF" << std::endl;
                playerState._patternToggleTimer = patternTimerMax;
            } else if (input.at(jt::KeyCode::Num3)) {
                playerState._shotPattern ^= Shots::ShotPattern::TRIPLE_WIDE();
                if ((playerState._shotPattern & Shots::ShotPattern::TRIPLE_WIDE())
                    == Shots::ShotPattern::TRIPLE_WIDE())
                    std::cout << "ShotPattern TRIPLE_WIDE: ON" << std::endl;
                else
                    std::cout << "ShotPattern TRIPLE_WIDE: OFF" << std::endl;
                playerState._patternToggleTimer = patternTimerMax;
            } else if (input.at(jt::KeyCode::Num4)) {
                playerState._shotPattern ^= Shots::ShotPattern::SIDE();
                if ((playerState._shotPattern & Shots::ShotPattern::SIDE())
                    == Shots::ShotPattern::SIDE())
                    std::cout << "ShotPattern SIDE: ON" << std::endl;
                else
                    std::cout << "ShotPattern SIDE: OFF" << std::endl;
                playerState._patternToggleTimer = patternTimerMax;
            } else if (input.at(jt::KeyCode::Num5)) {
                playerState._shotPattern ^= Shots::ShotPattern::BEHIND();
                if ((playerState._shotPattern & Shots::ShotPattern::BEHIND())
                    == Shots::ShotPattern::BEHIND())
                    std::cout << "ShotPattern BEHIND: ON" << std::endl;
                else
                    std::cout << "ShotPattern BEHIND: OFF" << std::endl;
                playerState._patternToggleTimer = patternTimerMax;
            }
        }

        playerState._shootTimer -= elapsed;
        playerState._patternToggleTimer -= elapsed;
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

void playerTakeDamage(PlayerState& playerState, ShotState& shot)
{
    if (playerState.health <= 0) {
        return;
    }
    playerState.health -= shot._damage;
    checkPlayerDeath(playerState);
}

// TODO take enemy size into account
void playerTakeDamage(PlayerState& playerState, EnemyState& enemy)
{
    if (playerState.health <= 0) {
        return;
    }
    playerState.health -= 4;
    checkPlayerDeath(playerState);
}
