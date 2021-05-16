#ifndef PLAYSTATE_GUARD_HPP
#define PLAYSTATE_GUARD_HPP

#include "GameProperties.hpp"
#include "InputState.hpp"
#include "SFML/Network/Packet.hpp"
#include "Vector.hpp"
#include <map>

struct ShotState;

struct PlayerState {
    jt::Vector2 position;
    float _shootTimer { 0 };
    int health { Game::GameProperties::playerMaxHealth() };
};

void updatePlayerState(PlayerState& playerState, float dt, InputState& input);
void playerTakeDamage(PlayerState& playerState, ShotState& shot);

using PlayerMap = std::map<int, PlayerState>;

sf::Packet& operator<<(sf::Packet&, PlayerState&);
sf::Packet& operator<<(sf::Packet&, PlayerState const&);
sf::Packet& operator>>(sf::Packet&, PlayerState&);

#endif
