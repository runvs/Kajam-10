#include "PlayerState.hpp"

#include "Conversions.hpp"
#include "common.hpp"

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

void updatePlayerState(PlayerState& playerState, float elapsed, InputState& input)
{
    if (input[jt::KeyCode::D])
        playerState.position.x() += elapsed * Game::GameProperties::PlayerMovementSpeed();
    else if (input[jt::KeyCode::A])
        playerState.position.x() -= elapsed * Game::GameProperties::PlayerMovementSpeed();

    if (input[jt::KeyCode::W])
        playerState.position.y() -= elapsed * Game::GameProperties::PlayerMovementSpeed();
    else if (input[jt::KeyCode::S])
        playerState.position.y() += elapsed * Game::GameProperties::PlayerMovementSpeed();
}
