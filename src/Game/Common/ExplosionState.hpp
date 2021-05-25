#ifndef EXPLOSIONSTATE_GUARD_HPP
#define EXPLOSIONSTATE_GUARD_HPP

#include "Vector.hpp"
#include <SFML/Network/Packet.hpp>
#include <chrono>

struct ExplosionState {
    jt::Vector2 position { 0.0f, 0.0f };

    std::chrono::steady_clock::time_point _begin { std::chrono::steady_clock::now() };
    float _progress { 0.0f };
    bool _alive { true };
};

sf::Packet& operator<<(sf::Packet& packet, ExplosionState& explosionState);
sf::Packet& operator<<(sf::Packet& packet, ExplosionState const& explosionState);
sf::Packet& operator>>(sf::Packet& packet, ExplosionState& explosionState);

void updateExplosionState(ExplosionState& state, float elapsed);

#endif // EXPLOSIONSTATE_GUARD_HPP
