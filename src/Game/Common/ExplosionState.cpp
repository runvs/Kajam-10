#include "ExplosionState.hpp"
#include "Conversions.hpp"
#include "GameProperties.hpp"

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

void updateExplosionState(ExplosionState& state, float elapsed)
{
    std::chrono::duration<float, std::milli> duration
        = std::chrono::steady_clock::now() - state._begin;

    state._progress = (duration.count() / 1000.0f) / Game::GameProperties::explosionDuration();
    state._alive = state._progress >= 1.0f;
}