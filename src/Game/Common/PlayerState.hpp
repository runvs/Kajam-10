﻿#ifndef PLAYSTATE_GUARD_HPP
#define PLAYSTATE_GUARD_HPP

#include "GameProperties.hpp"
#include "InputState.hpp"
#include "SFML/Network/Packet.hpp"
#include "Vector.hpp"
#include <map>

struct ShotState;
struct EnemyState;

struct PlayerState {
    jt::Vector2 position { 0, 0 };
    int health { 12 };

    float _shootTimer { 0 };
    float _respawnTimer { 0.0f };
};

void updatePlayerState(PlayerState& playerState, float dt, InputState const& input);
void playerTakeDamage(PlayerState& playerState, ShotState& shot);
void playerTakeDamage(PlayerState& playerState, EnemyState& enemy);

using PlayerMap = std::map<int, PlayerState>;

sf::Packet& operator<<(sf::Packet&, PlayerState&);
sf::Packet& operator<<(sf::Packet&, PlayerState const&);
sf::Packet& operator>>(sf::Packet&, PlayerState&);

#endif
