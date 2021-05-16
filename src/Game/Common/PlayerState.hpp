#ifndef PLAYSTATE_GUARD_HPP
#define PLAYSTATE_GUARD_HPP

#include "GameProperties.hpp"
#include "InputState.hpp"
#include "Random.hpp"
#include "SFML/Network/Packet.hpp"
#include "Vector.hpp"
#include <iostream>
#include <map>

struct ShotState;
struct EnemyState;

struct PlayerState {
    jt::Vector2 position { 200, 250 };
    float _shootTimer { 0 };
    int health { Game::GameProperties::playerMaxHealth() };
};

void updatePlayerState(PlayerState& playerState, float dt, InputState& input);
void playerTakeDamage(PlayerState& playerState, ShotState& shot);
void playerTakeDamage(PlayerState& playerState, EnemyState& enemy);

using PlayerMap = std::map<int, PlayerState>;

sf::Packet& operator<<(sf::Packet&, PlayerState&);
sf::Packet& operator<<(sf::Packet&, PlayerState const&);
sf::Packet& operator>>(sf::Packet&, PlayerState&);

#endif
