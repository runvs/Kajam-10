#ifndef SHOTSTATE_HPP_GUARD
#define SHOTSTATE_HPP_GUARD

#include "Vector.hpp"
#include <SFML/Network/Packet.hpp>

struct ShotState {
    jt::Vector2 position { -200.0f, -200.0f };
    jt::Vector2 direction { 0.0f, 1.0f };
    float _age { 0.0f };
    bool _alive { true };
    bool fromPlayer { true };
    int _damage { 1 };
};

sf::Packet& operator<<(sf::Packet& packet, ShotState& shotState);
sf::Packet& operator<<(sf::Packet& packet, ShotState const& shotState);
sf::Packet& operator>>(sf::Packet& packet, ShotState& shotState);

void updateShotState(ShotState& s, float elapsed);
jt::Vector2 getShotJitterDirection(float jitterAmount);
jt::Vector2 getShotJitterDirection(float jitterAmount, jt::Vector2 const& baseVector);

#endif
