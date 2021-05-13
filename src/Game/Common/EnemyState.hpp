#ifndef ENEMY_STATE_GUARD_HPP
#define ENEMY_STATE_GUARD_HPP

#include "Vector.hpp"
#include <SFML/Network/Packet.hpp>

struct EnemyState {
    jt::Vector2 position;
};

sf::Packet& operator<<(sf::Packet& packet, EnemyState& enemyState);
sf::Packet& operator<<(sf::Packet& packet, EnemyState const& enemyState);
sf::Packet& operator>>(sf::Packet& packet, EnemyState& enemyState);

#endif // ENEMY_STATE_GUARD_HPP
