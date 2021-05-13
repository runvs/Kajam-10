#ifndef ENEMY_STATE_GUARD_HPP
#define ENEMY_STATE_GUARD_HPP

#include "Vector.hpp"
#include <SFML/Network/Packet.hpp>

struct ShotState;

struct EnemyState {
    jt::Vector2 position;
    int _health;
    bool _alive;
};

sf::Packet& operator<<(sf::Packet& packet, EnemyState& enemyState);
sf::Packet& operator<<(sf::Packet& packet, EnemyState const& enemyState);
sf::Packet& operator>>(sf::Packet& packet, EnemyState& enemyState);

void enemyTakeDamage(EnemyState& enemyState, ShotState& shotState);

#endif // ENEMY_STATE_GUARD_HPP
