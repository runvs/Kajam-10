#ifndef ENEMY_STATE_GUARD_HPP
#define ENEMY_STATE_GUARD_HPP

#include "GameProperties.hpp"
#include "Vector.hpp"

#include <SFML/Network/Packet.hpp>
#include <memory>

struct ShotState;
class EnemyMoverBase;

struct EnemyState {
    jt::Vector2 position { -500.0f, -500.0f };
    int _health { Game::GameProperties::enemyDefaultHealth() };
    bool _alive { true };
    float _age { 0.0f };
    float _moveDelay { 0.0f };
    std::shared_ptr<EnemyMoverBase> _mover { nullptr };
    jt::Vector2 _positionBase { -500.0f, -500.0f };
};

sf::Packet& operator<<(sf::Packet& packet, EnemyState& enemyState);
sf::Packet& operator<<(sf::Packet& packet, EnemyState const& enemyState);
sf::Packet& operator>>(sf::Packet& packet, EnemyState& enemyState);

void enemyTakeDamage(EnemyState& enemyState, ShotState& shotState);
void updateEnemyState(EnemyState& enemyState, float elapsed);

#endif // ENEMY_STATE_GUARD_HPP
