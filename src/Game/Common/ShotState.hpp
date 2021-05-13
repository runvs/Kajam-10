﻿#ifndef SHOTSTATE_HPP_GUARD
#define SHOTSTATE_HPP_GUARD

#include "Vector.hpp"
#include <SFML/Network/Packet.hpp>

struct ShotState {
    jt::Vector2 position;
    jt::Vector2 direction;
    float _age { 0.0f };
};

sf::Packet& operator<<(sf::Packet& packet, ShotState& shotState);
sf::Packet& operator<<(sf::Packet& packet, ShotState const& shotState);
sf::Packet& operator>>(sf::Packet& packet, ShotState& shotState);

#endif
