#include "EnemyState.hpp"
#include "Conversions.hpp"
#include "EnemyMover.hpp"
#include "ShotState.hpp"

sf::Packet& operator<<(sf::Packet& packet, EnemyState& enemyState)
{
    return packet << enemyState.position;
}

sf::Packet& operator<<(sf::Packet& packet, EnemyState const& enemyState)
{
    return packet << enemyState.position;
}

sf::Packet& operator>>(sf::Packet& packet, EnemyState& enemyState)
{
    return packet >> enemyState.position;
}

void enemyTakeDamage(EnemyState& enemyState, ShotState& shotState)
{
    enemyState._health -= shotState._damage;
    enemyState._alive = enemyState._health > 0;
}

void updateEnemyState(EnemyState& enemyState, float elapsed)
{
    enemyState._age += elapsed;
    if (enemyState._mover) {
        enemyState._mover->move(enemyState, elapsed);
    }

    if (enemyState.position.y() >= Game::GameProperties::displayScreenSize().y() + 24) {
        enemyState._alive = false;
    }
    if (enemyState._age >= 25) {
        enemyState._alive = false;
    }
}
